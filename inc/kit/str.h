#pragma once

#define IsSpace(c)      ((c) == ' ' || (c) == '\t')
#define IsWhitespace(c) (IsSpace(c) || (c) == '\n')
#define IsDigit(c)      ((c) >= '0' && (c) <= '9')
#define IsUppercase(c)  ((c) >= 'A' && (c) <= 'Z')
#define IsLowercase(c)  ((c) >= 'a' && (c) <= 'z')
#define IsAlpha(c)      (IsUppercase(c) || IsLowercase(c))
#define IsSymChar(c)    (IsDigit(c) || IsAlpha(c))

void ClearStr(char *str);
u32 StrLen(char *str);
u32 NumDigits(i32 num, i32 base);
char *NumToString(i32 num, char *str);
char *HexToString(i32 num, char *str);
char *FormatStr(char *buf, char *str, u32 strLen);
char *FormatInt(char *buf, i32 num, u32 strLen);
char *StrCat(char *a, char *b);
u32 ParseInt(char **s);
bool StrEq(char *s1, char *s2);
