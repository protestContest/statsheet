#include "parse.h"
#include "vec.h"
#include "util.h"
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

void Adv(Parser *p)
{
  if (Peek(p) == '\n') {
    p->line++;
    p->col = 0;
  } else {
    p->col++;
  }
  p->cur++;
}

void SkipWhitespace(Parser *p)
{
  while (!AtEnd(p) && IsWhitespace(Peek(p))) {
    Adv(p);
  }
}

void SkipLine(Parser *p)
{
  while (!AtEnd(p) && Peek(p) != '\n') Adv(p);
  if (!AtEnd(p)) {
    Adv(p);
  }
}

void Expect(Parser *p, char *expected)
{
  char *orig = expected;
  while (!AtEnd(p) && *expected) {
    if (Peek(p) != *expected) {
      fprintf(stderr, "Expected \"%s\" at %s:%d:%d\n", orig, p->file, p->line+1, p->col+1);
      exit(99);
    }
    Adv(p);
    expected++;
  }
  if (*expected) {
    fprintf(stderr, "Unexpected EOF in %s", p->file);
    exit(99);
  }
}

bool Match(Parser *p, char *expected)
{
  char *orig = p->cur;
  while (!AtEnd(p) && *expected) {
    if (Peek(p) != *expected) {
      p->cur = orig;
      return false;
    }
    Adv(p);
    expected++;
  }
  if (*expected) {
    p->cur = orig;
    return false;
  }
  return true;
}

char *ParseName(Parser *p)
{
  char *start = p->cur;
  while (!AtEnd(p) && IsSymChar(Peek(p))) Adv(p);
  u32 len = p->cur - start;
  char *name = malloc(len + 1);
  strncpy(name, start, len);
  name[len] = 0;
  return name;
}

char *ParsePath(Parser *p)
{
  char *start = p->cur;
  while (!AtEnd(p) && !IsWhitespace(Peek(p))) Adv(p);
  u32 len = p->cur - start;
  char *name = malloc(len + 1);
  strncpy(name, start, len);
  name[len] = 0;
  return name;
}

u32 ParseID(Parser *p)
{
  char *start = p->cur;
  while (!AtEnd(p) && IsSymChar(Peek(p))) Adv(p);
  if (start == p->cur) {
    fprintf(stderr, "Expected ID at %s:%d:%d\n", p->file, p->line+1, p->col+1);
    exit(99);
  }
  return Hash(start, p->cur - start);
}

i32 ParseNum(Parser *p)
{
  i32 num = 0;
  i32 sign = 1;
  if (Peek(p) == '-') {
    sign = -1;
    Adv(p);

    if (AtEnd(p) || !IsDigit(Peek(p))) {
      fprintf(stderr, "Invalid number at %s:%d:%d\n", p->file, p->line+1, p->col+1);
      exit(99);
    }
  }

  while (!AtEnd(p) && IsDigit(Peek(p))) {
    num = num*10 + Peek(p) - '0';
    Adv(p);
  }
  return num * sign;
}

u32 ParseString(Parser *p, char **strings)
{
  u32 value = VecCount(*strings);
  Expect(p, "\"");
  char *strStart = p->cur;
  while (!AtEnd(p) && Peek(p) != '"') Adv(p);
  char *strEnd = p->cur;
  Expect(p, "\"");
  for (char *s = strStart; s < strEnd; s++) {
    VecPush(*strings, *s);
  }
  VecPush(*strings, 0);
  return value;
}
