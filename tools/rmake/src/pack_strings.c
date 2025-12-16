#include "pack_strings.h"
#include "parse.h"

void PackStrings(ResInfo *info, FILE *f)
{
  info->data = malloc(info->size + sizeof(u32));
  *((u32*)info->data) = info->size << 8;
  fread(info->data + sizeof(u32), info->size, 1, f);

  Parser p = {info->data + sizeof(u32), info->data + info->size + sizeof(u32), 0};
  u32 numSpells = 0;
  while (!AtEnd(&p)) {
    SkipWhitespace(&p);
    if (AtEnd(&p)) break;

    // header
    while (!AtEnd(&p) && *p.cur != '\n') {
      if (*p.cur == '\t') *p.cur = 0;
      p.cur++;
    }

    if (AtEnd(&p)) break;
    *p.cur = 0;
    p.cur++;

    // description
    while (!AtEnd(&p)) {
      if (p.end > p.cur && p.cur[0] == '\n' && p.cur[1] == '\n') {
        *p.cur = 0;
        p.cur++;
        numSpells++;
        break;
      }
      p.cur++;
    }
  }
  fprintf(stderr, "Parsed %d spells\n", numSpells);
}
