#pragma once

typedef struct {
  i16 ascent;
  i16 descent;
  i16 widMax;
  i16 leading;
} FontInfo;

void SetFont(char *name);
void GetFontInfo(FontInfo *info);
i16 LineHeight(void);
void Print(char *str);
void DrawChar(char c);
u32 CharWidth(char c);
u32 TextWidth(char *str);
u32 TextHeight(char *str);
