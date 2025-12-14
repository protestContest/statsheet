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

#define SkipSpaces(cur, end) do {\
  while ((cur) < (end) && IsSpace(*(cur))) (cur)++;\
} while(0)

u32 Hash(void *data, u32 size);
char *SkipWhitespace(char *cur, char *end);
char *ParseName(char **cur, char *end);
