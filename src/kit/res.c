#include "kit/res.h"
#include "kit/math.h"
#include "kit/str.h"
#include "kit/mem.h"
#include "kit/debug.h"

extern const ResFile __Resources__;
#define resFile (&__Resources__)

void *GetResource(char *name)
{
  u32 id = Hash(name, StrLen(name));
  for (u32 i = 0; i < resFile->count; i++) {
    if (resFile->resMap[i].id == id) {
      u8 *data = ((u8*)resFile) + resFile->resMap[i].offset;
      return Uncompress(data, 0);
    }
  }
  Assert(false);
  return 0;
}

void *Uncompress(void *src, void *dst)
{
  if (!src) return 0;

  u32 header = *((u32*)src);
  u8 type = (header & 0xFF) >> 4;
  u32 size = header >> 8;
  u8 *data = ((u8*)src) + sizeof(u32);

  if (type == Uncompressed && !dst) return data;

  if (!dst) dst = Alloc(size);

  switch (type) {
  case Uncompressed:
    Copy(data, dst, size);
    break;
  case Huffman:
    HuffUnComp((HuffData*)src, dst);
    break;
  case LZSS:
    LZSSUnComp((LZSSData*)src, dst);
    break;
  case RunLength:
    RLUnComp((RLData*)src, dst);
    break;
  case SubFilter:
    if ((header & 0x0F) == 1) {
      Diff8bitUnFilter(src, dst);
    } else if ((header & 0x0F) == 2) {
      Diff16bitUnFilter(src, dst);
    } else {
      return 0;
    }
    break;
  default:
    return 0;
  }

  return dst;
}
