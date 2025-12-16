#include "kit/mem.h"
#include "kit/debug.h"
#include "prefix.h"

/*
The heap is divided into free or allocated blocks. Each block begins with a 32-bit header. If the
header is positive, the block is allocated, and the header defines the number of words allocated
(not including the header). If the header is negative, the block is free, and the absolute value
defines the number of words free. On startup, the heap is initialized to have one free block
encompassing the entire 256K heap. A header of 0 should never occur - the space for that header
should be absorbed into its preceding block.

Allocation happens by seeking a free block big enough to fit the requested size. Until one is found,
the heap is scanned and any contiguous free blocks are gathered together. If the end of the heap is
reached, a null pointer is returned. When a block big enough for allocation is found, it's split
into an allocated block of the requested size and a free block of the remainder. A pointer to the
block data is returned.

Freeing happens by marking an allocated block as free (negating its header). To speed allocation,
subsequent contiguous free blocks are gathered into the freed block (but not free blocks before).

Reallocation has three possibilities:

- If the new size is smaller than the allocated size, the block is shrunk. A new free block is added
  after the allocated block.
- If the new size is larger than the allocated size and there is enough free space after the
  allocated block, the block is enlarged in place.
- Otherwise, (or if the given pointer is null), a new block is allocated and the old one is freed.
*/

extern i32 __HEAP_START__[];
extern i32 __HEAP_END__[];

#define FirstBlock          (((i32*)__HEAP_START__)+1)
#define BlockHeader(p)      (((i32*)(p))[-1])
#define AllocSize(p)        BlockHeader(p)
#define FreeSize(p)         (-BlockHeader(p))
#define BlockFree(p)        (BlockHeader(p) < 0)
#define NextBlock(p)        (((i32*)(p)) + (BlockFree(p) ? FreeSize(p) : AllocSize(p)) + 1)
#define InHeap(p)           (((void*)(p)) < (void*)__HEAP_END__ && ((void*)(p)) >= (void*)__HEAP_START__)
#define MarkFree(p, s)      do {BlockHeader(p) = -(s);} while(0)
#define MarkAlloc(p, s)     do {BlockHeader(p) = (s);} while(0)

// combines contiguous free blocks together
static void Gather(i32 *block)
{
  i32 *start = block;
  u32 freeSize = 0;

  while (InHeap(block) && BlockFree(block)) {
    freeSize += FreeSize(block) + 1;
    block = NextBlock(block);
  }

  if (freeSize > 0) {
    MarkFree(start, freeSize-1);
  }
}

// find the next free block big enough to contain size. gathers free blocks together.
static i32 *Seek(i32 *block, i32 size)
{
  while (InHeap(block) && !BlockFree(block)) {
    block = NextBlock(block);
  }
  if (!InHeap(block)) {
    Log("Heap exhausted");
    return 0;
  }

  Gather(block);
  if (FreeSize(block) < size) {
    return Seek(NextBlock(block), size);
  }

  return block;
}

// splits a block into two, the first allocated, the second free
// allocated size MUST NOT be greater than the block size
static void SplitBlock(i32 *block, i32 size)
{
  u32 blockSize = BlockFree(block) ? FreeSize(block) : AllocSize(block);
  u32 leftover = blockSize - size;
  if (leftover == 1) {
    // absorb empty free block
    leftover = 0;
    size++;
  }

  MarkAlloc(block, size);
  if (leftover > 0) {
    // next block guaranteed to exist
    MarkFree(NextBlock(block), leftover - 1);
    Gather(NextBlock(block));
  }
}

void *Alloc(u32 bytes)
{
  if (bytes == 0) return 0;

  i32 size = Align(bytes, 4)/4;  // convert size to words
  i32 *block = Seek(FirstBlock, size);  // find block big enough
  if (!block) {
    Error("Out of memory");
    return 0;
  }

  SplitBlock(block, size);  // split block into allocated/free

  return block;
}

void *Realloc(void *ptr, u32 bytes)
{
  if (bytes == 0) {
    Free(ptr);
    return 0;
  }

  i32 size = Align(bytes, 4)/4;

  if (ptr) {
    if (size == AllocSize(ptr)) {
      return ptr;
    } else if (size < AllocSize(ptr)) {
      // shrink in place
      SplitBlock(ptr, size);
      return ptr;
    } else {
      i32 *next = NextBlock(ptr);
      if (InHeap(next) && BlockFree(next)) {
        Gather(next);
        if (AllocSize(ptr) + FreeSize(next) + 1 >= size) {
          // grow in place
          MarkFree(ptr, AllocSize(ptr) + FreeSize(next) + 1);
          SplitBlock(ptr, size);
          return ptr;
        }
      }
    }
  }

  void *newBlock = Alloc(bytes);
  if (!newBlock) return 0;
  if (ptr) {
    Copy(ptr, newBlock, AllocSize(ptr)*4);
    Free(ptr);
  }
  return newBlock;
}

void Free(void *ptr)
{
  if (!InHeap(ptr) || BlockFree(ptr)) return;
  MarkFree(ptr, AllocSize(ptr));
  Gather(ptr);
}

static bool CanBlockCopy(void *src, void *dst, u32 size)
{
  if (size < 32) return false;
  if ((u32)src != Align(src, 4)) return false;
  if ((u32)dst != Align(dst, 4)) return false;
  if (dst > src && dst < src + size) return false;
  return true;
}

void Copy(void *src, void *dst, u32 size)
{
  u8 *srcBytes = (u8*)src;
  u8 *dstBytes = (u8*)dst;

  if (CanBlockCopy(src, dst, size)) {
    BlockCopy(srcBytes, dstBytes, size/4);
    srcBytes += size & ~3;
    dstBytes += size & ~3;

    for (u32 i = 0; i < (size & 0x3); i++) {
      *dstBytes++ = *srcBytes++;
    }
    return;
  }

  if (dst > src) {
    for (u32 i = 0; i < size; i++) dstBytes[size-1-i] = srcBytes[size-1-i];
  } else {
    for (u32 i = 0; i < size; i++) *dstBytes++ = *srcBytes++;
  }
}
