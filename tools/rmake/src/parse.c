#include "parse.h"
#include "vec.h"
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

void SkipWhitespace(Parser *p)
{
  while (p->cur < p->end && IsWhitespace(*p->cur)) {
    if (*p->cur == '\n') p->line++;
    p->cur++;
  }
}

void SkipLine(Parser *p)
{
  while (!AtEnd(p) && *p->cur != '\n') p->cur++;
  if (!AtEnd(p)) {
    p->cur++;
    p->line++;
  }
}

void Expect(Parser *p, char *expected)
{
  char *orig = expected;
  while (!AtEnd(p) && *expected) {
    if (*p->cur != *expected) {
      fprintf(stderr, "Expected \"%s\" on line %d\n", orig, p->line+1);
      exit(99);
    }
    if (*p->cur == '\n') p->line++;
    p->cur++;
    expected++;
  }
  if (*expected) {
    fprintf(stderr, "Unexpected EOF");
    exit(99);
  }
}

char *ParseName(Parser *p)
{
  char *start = p->cur;
  while (p->cur < p->end && !IsWhitespace(*p->cur) && *p->cur != ':') (p->cur)++;
  u32 len = p->cur - start;
  char *name = malloc(len + 1);
  strncpy(name, start, len);
  name[len] = 0;
  return name;
}

u32 ParseID(Parser *p)
{
  char *start = p->cur;
  while (!AtEnd(p) && IsSymChar(*p->cur)) p->cur++;
  if (start == p->cur) {
    fprintf(stderr, "Expected ID\n");
    exit(99);
  }
  return Hash(start, p->cur - start);
}

i32 ParseNum(Parser *p)
{
  i32 num = 0;
  i32 sign = 1;
  if (*p->cur == '-') {
    sign = -1;
    (p->cur)++;
  }
  if (!IsDigit(*p->cur)) {
    fprintf(stderr, "Invalid integer\n");
    exit(99);
  }
  while (!AtEnd(p) && IsDigit(*p->cur)) {
    num = num*10 + *p->cur - '0';
    p->cur++;
  }
  return num * sign;
}

u32 ParseString(Parser *p, char **strings)
{
  u32 value = VecCount(*strings);
  Expect(p, "\"");
  char *strStart = p->cur;
  while (!AtEnd(p) && *p->cur != '"') p->cur++;
  char *strEnd = p->cur;
  Expect(p, "\"");
  for (char *s = strStart; s < strEnd; s++) {
    VecPush(*strings, *s);
  }
  VecPush(*strings, 0);
  return value;
}
