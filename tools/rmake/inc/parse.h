#pragma once
#include <stdio.h>
#include <stdlib.h>

#define IsSpace(c)      ((c) == ' ' || (c) == '\t')
#define IsWhitespace(c) (IsSpace(c) || (c) == '\n')
#define IsDigit(c)      ((c) >= '0' && (c) <= '9')
#define IsUppercase(c)  ((c) >= 'A' && (c) <= 'Z')
#define IsLowercase(c)  ((c) >= 'a' && (c) <= 'z')
#define IsAlpha(c)      (IsUppercase(c) || IsLowercase(c))
#define IsSymChar(c)    (IsDigit(c) || IsAlpha(c) || (c) == '_')

#define AtEnd(p)  ((p)->cur >= (p)->end)
#define Peek(p)   (*(p)->cur)
#define SkipSpaces(p) do {\
  while ((p)->cur < (p)->end && IsSpace(*(p)->cur)) (p)->cur++;\
} while(0)

typedef struct {
  char *cur;
  char *end;
  char *file;
  u32 line;
  u32 col;
} Parser;

void Adv(Parser *p);
void SkipWhitespace(Parser *p);
void SkipLine(Parser *p);
void Expect(Parser *p, char *expected);
bool Match(Parser *p, char *expected);
char *ParseName(Parser *p);
char *ParsePath(Parser *p);
u32 ParseID(Parser *p);
i32 ParseNum(Parser *p);
u32 ParseString(Parser *p, char **strings);
