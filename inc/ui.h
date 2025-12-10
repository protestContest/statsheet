#pragma once
#include "kit.h"

#define BG  RGB(128,184,128)

#define arrowLeftObj  1
#define arrowRightObj 2

void InitUI(void);
void ShowWindow(Rect *bounds);
void TextWindow(char *text, Rect *bounds);
void Alert(char *text);
void ShowCursor(void);
void HideCursor(void);
void PlaceCursor(i16 x, i16 y);
void ShowArrows(Rect *bounds);
void HideArrows(void);
void DrawNum(i32 num, i16 x, i16 y, bool showSign);
void DrawCharges(i32 used, i32 max, Rect *bounds);
i32 EditNum(i32 num, Rect *bounds, bool showSign);
i32 EditCharges(i32 used, i32 max, Rect *bounds);
void HideAllObjs(void);
