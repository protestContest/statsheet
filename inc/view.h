#pragma once
#include "kit.h"

struct View;
typedef void (*ViewDraw)(struct View *view);
typedef void (*ViewInput)(struct View *view, u16 input);
typedef void (*ViewActivate)(struct View *view, bool active);
typedef struct View {
  Rect bounds;
  ViewDraw draw;
  ViewInput onInput;
  ViewActivate activate;
} View;

void InitView(View *view, Rect *bounds, ViewDraw draw, ViewInput onInput, ViewActivate activate);
