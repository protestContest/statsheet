#include "pack_strings.h"
#include "parse.h"

void PackStrings(ResInfo *info, FILE *f)
{
  info->data = malloc(info->size + sizeof(u32));
  *((u32*)info->data) = info->size << 8;
  fread(info->data + sizeof(u32), info->size, 1, f);

  char *end = (char*)info->data + info->size + sizeof(u32);
  char *cur = (char*)info->data + sizeof(u32);
  u32 numSpells = 0;
  while (cur < end) {
    cur = SkipWhitespace(cur, end);
    if (cur == end) break;

    // header
    while (cur < end && *cur != '\n') {
      if (*cur == '\t') *cur = 0;
      cur++;
    }

    if (cur == end) break;
    *cur = 0;
    cur++;

    // description
    while (cur < end) {
      if (cur < end-1 && cur[0] == '\n' && cur[1] == '\n') {
        *cur = 0;
        cur++;
        numSpells++;
        break;
      }
      cur++;
    }
  }
  fprintf(stderr, "Parsed %d spells\n", numSpells);
}
