#include "kit/vec.h"
#include "kit/mem.h"
#include "kit/debug.h"

void InitVec(Vec *vec, u32 itemSize, u32 capacity)
{
  vec->capacity = capacity;
  vec->count = 0;
  vec->itemSize = itemSize;
  vec->items = 0;
  if (capacity > 0) {
    ResizeVec(vec, capacity);
  }
}

void ResizeVec(Vec *vec, u32 capacity)
{
  vec->capacity = capacity;
  vec->items = Realloc(vec->items, vec->itemSize*vec->capacity);
}

void VecPush(Vec *vec, void *item)
{
  if (vec->count + 1 > vec->capacity) {
    vec->capacity = vec->capacity ? 2*vec->capacity : 2;
    vec->items = Realloc(vec->items, vec->itemSize*vec->capacity);
    Assert(vec->items);
  }

  u32 index = vec->count++;
  Copy(item, VecAt(vec, index), vec->itemSize);
}

void VecPop(Vec *vec, void *dst)
{
  if (vec->count == 0) return;
  if (dst) {
    VecGet(vec, vec->count-1, dst);
  }
  vec->count--;
}

void VecGet(Vec *vec, u32 index, void *dst)
{
  if (index >= vec->count) return;
  Copy(VecAt(vec, index), dst, vec->itemSize);
}

void *VecAt(Vec *vec, u32 index)
{
  if (index >= vec->count) return 0;
  return ((u8*)vec->items) + index*vec->itemSize;
}
