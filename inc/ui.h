#pragma once
#include "kit.h"

#define arrowLeftObj  1
#define arrowRightObj 2

void InitUI(void);
void ShowWindow(Rect *bounds);
void TextWindow(char *text, i16 x, i16 y);
void Alert(char *text);
