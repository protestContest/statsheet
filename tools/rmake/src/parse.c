#include "parse.h"
#include <string.h>

u32 Hash(void *data, u32 size)
{
  u32 hash = 5381;
  u32 i;
  u8 *bytes = (u8*)data;
  for (i = 0; i < size; i++) {
    hash = ((hash << 5) + hash) + bytes[i];
  }
  return hash;
}

char *SkipWhitespace(char *cur, char *end)
{
  while (cur < end && IsWhitespace(*cur)) cur++;
  return cur;
}

char *ParseName(char **cur, char *end)
{
  char *start = *cur;
  while (*cur < end && !IsWhitespace(**cur) && **cur != ':') (*cur)++;
  u32 len = *cur - start;
  char *name = malloc(len + 1);
  strncpy(name, start, len);
  name[len] = 0;
  return name;
}
