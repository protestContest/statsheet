#include "kit/str.h"
#include "kit/mem.h"

void ClearStr(char *str)
{
  while (*str != 0) *str++ = 0;
}

u32 StrLen(char *s)
{
  u32 len = 0;
  while (*s) {
    len++;
    s++;
  }
  return len;
}

u32 NumDigits(i32 num, i32 base)
{
  u32 digits = 0;
  if (num == 0) return 1;
  if (num < 0) {
    digits = 1;
    num = -num;
  }
  while (num > 0) {
    num /= base;
    digits++;
  }
  return digits;
}

char *NumToString(i32 num, char *str)
{
  if (num == 0) {
    *str = '0';
    return str+1;
  }
  if (num < 0) {
    *str = '-';
    str++;
    num = -num;
  }

  u32 numDigits = NumDigits(num, 10);

  for (u32 i = 0; i < numDigits; i++) {
    u32 digit = num % 10;
    str[numDigits-i-1] = digit + '0';
    num /= 10;
  }
  return str+numDigits;
}

char *HexToString(i32 num, char *str)
{
  if (num == 0) {
    *str = '0';
    *(str+1) = '0';
    return str+2;
  }

  u32 numDigits = NumDigits(num, 16);
  for (u32 i = 0; i < numDigits; i++) {
    u32 digit = num % 16;
    str[numDigits-i-1] = (digit < 10) ? digit + '0' : digit - 10 + 'A';
    num /= 16;
  }
  return str + numDigits;
}

char *FormatStr(char *buf, char *str, u32 max)
{
  u32 len = StrLen(str);
  while (max > 0) {
    if (*buf == '^') {
      if (max < len) return 0;
      Copy(buf + 1, buf + len, max - len);
      Copy(str, buf, len);
      return buf + len;
    }
    buf++;
    max--;
  }
  return buf;
}

char *FormatInt(char *buf, i32 num, u32 strLen)
{
  u32 numDigits = NumDigits(num, 10);
  while (strLen > 0) {
    if (*buf == '^') {
      if (strLen < numDigits) return 0;
      Copy(buf + 1, buf + numDigits, strLen - numDigits);
      return NumToString(num, buf);
    }
    buf++;
    strLen--;
  }
  return buf;
}

char *StrCat(char *a, char *b)
{
  u32 alen = StrLen(a);
  u32 blen = StrLen(b);
  Copy(b, a+alen, blen);
  return a + alen + blen;
}

u32 ParseInt(char **s)
{
  u32 n = 0;
  while (IsDigit(**s)) {
    n = n*10 + **s - '0';
    (*s)++;
  }
  return n;
}

bool StrEq(char *s1, char *s2)
{
  while (*s1 && *s2 && *s1 == *s2) {
    s1++;
    s2++;
  }
  return *s1 == *s2;
}
