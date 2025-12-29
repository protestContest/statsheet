#include "lzss.h"
#include "manifest.h"
#include "util.h"
#include "../../inc/kit/res.h"
#include <string.h>

#define WINDOW_SIZE 4096
#define MIN_MATCH   3
#define MAX_MATCH   18
#define HASH_BITS   12
#define HASH_SIZE   (1 << HASH_BITS)
#define HASH_MASK   (HASH_SIZE-1)
#define NO_MATCH    (-1)

typedef struct {
  u16 pos;
  u16 length;
} Match;

static Match BuildMatch(u32 pos, u8 *data, u32 index, u32 size)
{
  Match match = {0};
  match.pos = pos;
  for (u32 i = 0; i < MAX_MATCH && index+i < size; i++) {
    if (data[index+i] != data[match.pos+i]) break;
    match.length++;
  }
  return match;
}

static Match FindMatch(u16 hash, u32 *head, u32 *prev, u8 *data, u32 index, u32 size)
{
  Match match = {0};
  Match longest = {0};

  if (head[hash] == NO_MATCH) return match;

  match = BuildMatch(head[hash], data, index, size);
  if (index - match.pos > WINDOW_SIZE) return longest;
  longest = match;

  while (prev[match.pos%WINDOW_SIZE] != NO_MATCH) {
    match = BuildMatch(prev[match.pos%WINDOW_SIZE], data, index, size);
    if (index - match.pos > WINDOW_SIZE) break;

    if (match.length > longest.length) {
      longest = match;
      if (match.length == MAX_MATCH) break;
    }
  }

  return longest;
}

typedef struct {
  u32 byte;
  u32 bit;
} BitCursor;

static void WriteMatch(Match match, ResInfo *info, u8 *data, u32 index, BitCursor *flags)
{
  if (flags->bit == 8) {
    flags->bit = 0;
    flags->byte = info->size++;
    info->data[flags->byte] = 0;
  }

  if (match.length >= MIN_MATCH) {
    info->data[flags->byte] |= 1 << (7 - flags->bit);
    u8 len = (match.length - MIN_MATCH) << 4;
    u16 offset = index - match.pos - 1;
    u8 offset1 = (offset >> 8) & 0xFF;
    u8 offset2 = offset & 0xFF;
    info->data[info->size++] = len | offset1;
    info->data[info->size++] = offset2;
  } else {
    info->data[info->size++] = data[index];
  }

  flags->bit++;
}

void LZSSEncode(u8 *data, ResInfo *info)
{
  u32 head[HASH_SIZE];   // hash -> pos of last match
  u32 prev[WINDOW_SIZE]; // pos -> pos of prev match
  memset(head, NO_MATCH, sizeof(head));
  memset(prev, NO_MATCH, sizeof(prev));

  u32 dataSize = info->size;
  info->data = malloc(dataSize + dataSize/8 + 16);
  info->size = sizeof(u32);

  BitCursor flags;
  flags.byte = info->size++;
  info->data[flags.byte] = 0;
  flags.bit = 0;

  u32 i = 0;
  while (i + MIN_MATCH <= dataSize) {
    u16 hash = Hash(data + i, MIN_MATCH) & HASH_MASK;
    Match match = FindMatch(hash, head, prev, data, i, dataSize);
    WriteMatch(match, info, data, i, &flags);

    prev[i % WINDOW_SIZE] = head[hash];
    head[hash] = i;

    if (match.length < MIN_MATCH) {
      i++;
    } else {
      for (u32 j = 1; j+i+MIN_MATCH <= dataSize && j < match.length; j++) {
        hash = Hash(data+i+j, MIN_MATCH) & HASH_MASK;
        prev[(i+j) % WINDOW_SIZE] = head[hash];
        head[hash] = i+j;
      }
      i += match.length;
    }
  }

  while (i < dataSize) {
    Match match = {0, 0};
    WriteMatch(match, info, data, i, &flags);
    i++;
  }
  while (info->size % 4 != 0) {
    info->data[info->size++] = 0;
  }

  *((u32*)info->data) = (dataSize << 8) | (LZSS << 4);
  info->data = realloc(info->data, info->size);
}

static void EncodeTiny(u8 *data, ResInfo *info)
{
  u32 size = Align(sizeof(u8) + info->size, 4);
  info->data = malloc(size + sizeof(u32));
  memset(info->data, 0, size + sizeof(u32));

  *((u32*)info->data) = (info->size << 8) | (LZSS << 4);

  info->data[sizeof(u32)] = 0;

  for (u32 i = 0; i < info->size; i++) {
    info->data[sizeof(u32) + sizeof(u8) + i] = data[i];
  }
  info->size = size + sizeof(u32);
}

void PackLZSS(ResInfo *info)
{
  FILE *f = fopen(info->path, "rb");
  u8 *data = malloc(info->size);
  fread(data, info->size, 1, f);

  if (info->size < 3) {
    EncodeTiny(data, info);
  } else {
    LZSSEncode(data, info);
  }

  free(data);
  fclose(f);
}
