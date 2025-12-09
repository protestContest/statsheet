#include "runlength.h"
#include "../../inc/kit/res.h"

void PackRunLength(ResInfo *info, FILE *f)
{
  u8 *data = malloc(info->size);
  fread(data, info->size, 1, f);

  u32 header = (info->size << 8) | (RunLength << 4);
  u32 size = sizeof(header);

  u32 start, end = 0;
  while (end < info->size) {
    start = end;
    while (
        end < info->size &&
        (end + 3 > info->size || data[end] != data[end+1] || data[end] != data[end+2]) &&
        end - start < 128) {
      end++;
    }
    if (end - start > 0) {
      size += end - start + 1;
    }

    start = end;
    while (end < info->size && data[end] == data[start] && end - start < 130) {
      end++;
    }
    if (end - start > 2) {
      size += 2;
      start = end;
    } else {
      end = start;
    }
  }

  u8 *encoded = malloc(Align(size, 4));
  *((u32*)encoded) = header;
  u8 *cur = encoded + sizeof(header);

  end = 0;
  while (end < info->size) {
    start = end;
    while (
        end < info->size &&
        (end > info->size - 3 || data[end] != data[end+1] || data[end] != data[end+2]) &&
        end - start < 128) {
      end++;
    }
    if (end - start > 0) {
      *cur++ = end - start - 1;
      for (u32 i = start; i < end; i++) {
        *cur++ = data[i];
      }
    }

    start = end;
    while (end < info->size && data[end] == data[start] && end - start < 130) {
      end++;
    }
    if (end - start > 2) {
      *cur++ = (end - start - 3) | 0x80;
      *cur++ = data[start];
    } else {
      end = start;
    }
  }

  for (u32 i = size; i < Align(size, 4); i++) {
    encoded[i] = 0;
  }

  info->data = encoded;
  info->size = size;

  free(data);
}
