#include "lzss.h"
#include "manifest.h"
#include "util.h"
#include "../../inc/kit/res.h"
#include <string.h>

/*
The compressed stream contains a sequence of "blocks". Each block is either a literal byte, or a
16-bit backreference. The backreference is split into a 4-bit length and 12-bit offset. The length
is the backreference length minus 3, and the offset is the distance backwards minus one, giving a
length range of 3-18 and a distance range of 1-4096.

Blocks appear in the stream in groups of eight. Each group is preceded by a byte whose bits indicate
the type of each block: 0 for byte literal or 1 for backreference.

This compressor uses a "linked hash" scheme. The compressor scans the input data and outputs either
literal bytes or backreferences. At each byte, it must search for a matching string of at least 3 in
the window (the previous 4096 bytes). To speed up this process, the compressor maintains a hash
table of the last position seen of every trigram. It also maintains a linked list of previously seen
trigrams. Instead of scanning the entire window for matches, the compressor only needs to scan the
list of matching trigrams.
*/

#define LEN_BITS    4
#define DIST_BITS   12
#define MIN_MATCH   3

#define WINDOW_SIZE (1 << DIST_BITS)
#define MAX_MATCH   ((1 << LEN_BITS) - 1 + MIN_MATCH)

#define HASH_BITS   12
#define HASH_SIZE   (1 << HASH_BITS)
#define HASH_MASK   (HASH_SIZE-1)
#define NO_MATCH    (-1)
#define EMPTY_MATCH (Match){0, 1}

#define HashTrigram(data)         (Hash((data), MIN_MATCH) & HASH_MASK)
#define OutsideWindow(pos, cur)   ((cur) - (pos) > WINDOW_SIZE)

typedef struct {
  u32 capacity;
  u32 index;
  u8 *data;
} DataStream;

typedef struct {
  u16 pos;
  u16 length;
} Match;

typedef struct {
  u32 byte;
  u32 bit;
} BitCursor;

typedef struct {
  u32 head[HASH_SIZE];   // hash -> pos of last match
  u32 prev[WINDOW_SIZE]; // pos -> pos of prev match
} HashChain;

static void InitHashChain(HashChain *chain)
{
  memset(chain->head, NO_MATCH, sizeof(chain->head));
  memset(chain->prev, NO_MATCH, sizeof(chain->prev));
}

static void InitDataStream(DataStream *stream, u8 *data, u32 index, u32 capacity)
{
  stream->data = data;
  stream->index = index;
  stream->capacity = capacity;
}

/* append a new flag byte in the output */
static void AdvanceBitCursor(BitCursor *cursor, DataStream *output)
{
  cursor->bit = 0;
  cursor->byte = output->index++;
  output->data[cursor->byte] = 0;
}

/* build a match at a given backreference position */
static Match BuildMatch(u32 pos, DataStream *input, u32 index)
{
  Match match = {pos, 0};

  for (u32 i = 0; i < MAX_MATCH && index + i < input->capacity; i++) {
    if (input->data[index + i] != input->data[match.pos + i]) break;
    match.length++;
  }
  return match;
}

/* find the longest match in the hash chain of the current index */
static Match FindMatch(u16 hash, HashChain *chain, DataStream *input, u32 index)
{
  Match match, longest;

  // no match
  if (chain->head[hash] == NO_MATCH) return EMPTY_MATCH;

  // build first match from head
  match = BuildMatch(chain->head[hash], input, index);
  if (OutsideWindow(match.pos, index)) return EMPTY_MATCH;
  longest = match;

  // search the hash chain for a longer match
  while (chain->prev[match.pos % WINDOW_SIZE] != NO_MATCH) {
    match = BuildMatch(chain->prev[match.pos % WINDOW_SIZE], input, index);
    if (OutsideWindow(match.pos, index)) break;

    if (match.length > longest.length) {
      longest = match;
      if (match.length == MAX_MATCH) break;
    }
  }

  if (longest.length < MIN_MATCH) {
    return EMPTY_MATCH;
  }

  return longest;
}

/* output either a byte literal or backreference, and set the flag */
static void WriteMatch(Match match, DataStream *input, DataStream *output, BitCursor *flags)
{
  if (flags->bit == 8) {
    AdvanceBitCursor(flags, output);
  }

  if (match.length >= MIN_MATCH) {
    output->data[flags->byte] |= 1 << (7 - flags->bit);
    u8 len = (match.length - MIN_MATCH) << LEN_BITS;
    u16 offset = input->index - match.pos - 1;
    u8 offset1 = (offset >> 8) & 0x0F;
    u8 offset2 = offset & 0xFF;
    output->data[output->index++] = len | offset1;
    output->data[output->index++] = offset2;
  } else {
    output->data[output->index++] = input->data[input->index];
  }

  flags->bit++;
}

/* for each input byte in the match, add the trigram hash to the hash chain */
static void UpdateHashChain(HashChain *chain, Match match, DataStream *input)
{
  u32 end = Min(input->index + match.length, input->capacity - MIN_MATCH + 1);

  for (u32 i = input->index; i < end; i++) {
    u16 hash = HashTrigram(input->data + i);
    chain->prev[i % WINDOW_SIZE] = chain->head[hash];
    chain->head[hash] = i;
  }

  input->index += match.length;
}

static void LZSSEncode(DataStream *input, DataStream *output)
{
  HashChain chain;
  BitCursor flags;

  InitHashChain(&chain);
  AdvanceBitCursor(&flags, output);

  // scan input data and output compressed stream (up to the point we can have a backreference)
  while (input->index + MIN_MATCH <= input->capacity) {
    u16 hash = HashTrigram(input->data + input->index);
    Match match = FindMatch(hash, &chain, input, input->index);

    // check for a longer match one byte later
    if (input->index + 1 + MIN_MATCH <= input->capacity) {
      u16 hash2 = HashTrigram(input->data + input->index + 1);
      Match match2 = FindMatch(hash2, &chain, input, input->index + 1);
      if (match2.length > match.length + 1) {
        // process current match as an empty match, then replace it with the better match
        match.length = 1;
        WriteMatch(match, input, output, &flags);
        UpdateHashChain(&chain, match, input);
        match = match2;
      }
    }

    WriteMatch(match, input, output, &flags);
    UpdateHashChain(&chain, match, input);
  }

  // add literal blocks for the last bytes of input
  while (input->index < input->capacity) {
    Match match = EMPTY_MATCH;
    WriteMatch(match, input, output, &flags);
    input->index++;
  }

  // pad output to 4 bytes
  while (output->index % 4 != 0) {
    output->data[output->index++] = 0;
  }
}

u32 CompressLZSS(void *src, u32 length, u8 **dst)
{
  DataStream input, output;
  InitDataStream(&input, src, 0, length);

  // initialize output
  u32 maxOutputSize = length + length/8 + 16;
  InitDataStream(&output, malloc(maxOutputSize), 0, maxOutputSize);

  // write header
  WriteHeader(output.data, ResHeader(length, LZSS, 0));
  output.index += sizeof(u32);

  LZSSEncode(&input, &output);

  *dst = realloc(output.data, output.index);
  return output.index;
}

void PackLZSS(ResInfo *info)
{
  u8 *data = malloc(info->size);

  // read file into input
  FILE *f = fopen(info->path, "rb");
  fread(data, info->size, 1, f);
  fclose(f);

  info->size = CompressLZSS(data, info->size, &info->data);
  free(data);
}
