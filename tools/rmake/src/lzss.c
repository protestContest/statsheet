#include "lzss.h"
#include "manifest.h"

void LZSSEncode(u8 *data, ResInfo *info)
{
}

void PackLZSS(ResInfo *info)
{
  FILE *f = fopen(info->path, "rb");
  u8 *data = malloc(info->size);
  fread(data, info->size, 1, f);
  LZSSEncode(data, info);
  free(data);
  fclose(f);
}
