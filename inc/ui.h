#pragma once
#include "kit.h"

#define arrowLeftObj  1
#define arrowRightObj 2

void InitUI(void);
void ShowWindow(Rect *bounds);
void TextWindow(char *text, i16 x, i16 y);
void Alert(char *text);
void ShowCursor(void);
void HideCursor(void);
void PlaceCursor(i16 x, i16 y);

void DrawNum(i32 num, i16 x, i16 y, bool showSign);
void DrawCharges(i32 used, i32 max, Rect *bounds);
i32 EditNum(i32 num, Rect *bounds, bool showSign);
i32 EditCharges(i32 used, i32 max, Rect *bounds);
