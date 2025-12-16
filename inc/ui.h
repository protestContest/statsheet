#pragma once
#include "kit/canvas.h"

#define BG      RGB(128,184,128)
#define WIN_BG  RGB(200,200,200)

#define arrowLeftObj  1
#define arrowRightObj 2
#define arrowUpObj    3
#define arrowDownObj  4

void InitUI(void);
void DrawWindow(Rect *bounds);
void Alert(char *text);
void ShowCursor(void);
void HideCursor(void);
void PlaceCursor(i16 x, i16 y);
void ShowArrows(Rect *bounds, VHSelect selectDir);
void HideArrows(void);
void HideAllObjs(void);

// void DrawNum(i32 num, i16 x, i16 y, bool showSign);
// void DrawCharges(i32 used, i32 max, Rect *bounds);
// i32 NumInput(i32 num, u16 input, Rect *bounds, bool showSign, bool selectDir);
// i32 EditNum(i32 num, Rect *bounds, bool showSign, bool selectDir);
// i32 EditCharges(i32 used, i32 max, Rect *bounds);
