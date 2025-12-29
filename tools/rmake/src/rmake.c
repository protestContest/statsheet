#include "../../inc/kit/res.h"
#include <libgen.h>
#include <string.h>
#include <stdlib.h>

#include "manifest.h"
#include "huffman.h"
#include "lzss.h"
#include "runlength.h"
#include "pack_stats.h"
#include "util.h"

/*
Reads a manifest file and packs resources into a resource file. The manifest file is a list of
resource specs, one per line. Each resource spec specifies the relative path of the resource
followed by an optional packing method:

  Z: Compress with LZSS
  h: Compress with Huffman, 4-bit
  H: Compress with Huffman, 8-bit
  R: Compress with run-length

*/

typedef void (*PackFn)(ResInfo *info);

typedef struct {
  char *name;
  PackFn pack;
} PackMethod;

static PackMethod methods[] = {
  {"Z", PackLZSS},
  {"H", PackHuffman},
  {"R", PackRunLength},
  {"PackStats", PackStats},
};

void Filter(u8 *data, u32 size, u32 dataSize)
{
  if (dataSize == 2) {
    i16 *words = (i16*)data;
    size >>= 1;
    i16 prev = words[0];
    for (u32 i = 1; i < size; i++) {
      i16 cur = words[i];
      words[i] = cur - prev;
      prev = cur;
    }
  } else {
    i8 *bytes = (i8*)data;
    i8 prev = bytes[0];
    for (u32 i = 1; i < size; i++) {
      i8 cur = bytes[i];
      bytes[i] = cur - prev;
      prev = cur;
    }
  }
}

void EncodeItem(ResInfo *info)
{
  info->size = FileSize(info->path);

  if (info->packMethod) {
    for (u32 i = 0; i < ArrayCount(methods); i++) {
      if (strcmp(methods[i].name, info->packMethod) == 0) {
        methods[i].pack(info);
        break;
      }
    }
  }
  if (!info->data) {
    FILE *f = fopen(info->path, "rb");
    info->data = malloc(info->size + sizeof(u32));
    *((u32*)info->data) = info->size << 8;
    fread(info->data + sizeof(u32), info->size, 1, f);
    fclose(f);
    info->size += sizeof(u32);
  }
}

ResFile *NewResFile(Manifest *manifest)
{
  u32 mapSize = sizeof(ResMapItem)*manifest->count;
  u32 length = sizeof(ResFile) + mapSize;
  ResFile *resFile = malloc(length);
  resFile->count = manifest->count;
  resFile->length = length;
  return resFile;
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Usage: rmake Manifest\n");
    return 1;
  }

  Manifest *manifest = ParseManifest(argv[1]);

  for (u32 i = 0; i < manifest->count; i++) {
    EncodeItem(&manifest->items[i]);
  }

  ResFile *resFile = NewResFile(manifest);
  for (u32 i = 0; i < manifest->count; i++) {
    resFile->resMap[i].id = manifest->items[i].id;
    resFile->resMap[i].offset = resFile->length;
    resFile->length += Align(manifest->items[i].size, 4);
  }
  fwrite(resFile, sizeof(ResFile) + sizeof(ResMapItem)*manifest->count, 1, stdout);

  for (u32 i = 0; i < manifest->count; i++) {
    u32 zero = 0;
    u32 padSize = Align(manifest->items[i].size, 4) - manifest->items[i].size;
    fwrite(manifest->items[i].data, manifest->items[i].size, 1, stdout);
    fwrite(&zero, padSize, 1, stdout);
  }

  return 0;
}
