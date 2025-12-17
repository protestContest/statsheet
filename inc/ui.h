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
void ShowUpArrow(i16 x, i16 y);
void ShowDownArrow(i16 x, i16 y);
void HideArrow(Direction arrow);
void HideArrows(void);
void HideAllObjs(void);
