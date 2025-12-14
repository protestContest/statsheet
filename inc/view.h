#pragma once
#include "kit/canvas.h"

struct View;
typedef void (*ViewDraw)(struct View *view);
typedef bool (*ViewInput)(struct View *view, u16 input);
typedef void (*ViewActivate)(struct View *view, bool active);
typedef struct View {
  Rect bounds;
  ViewDraw draw;
  ViewInput onInput;
  ViewActivate activate;
} View;
#define DrawView(v)       ((v)->draw && ((v)->draw(v),0))
#define InputView(v,i)    ((v)->onInput && (v)->onInput(v,i))
#define ActivateView(v,a) ((v)->activate && ((v)->activate(v,a),0))

void InitView(View *view, Rect *bounds, ViewDraw draw, ViewInput onInput, ViewActivate activate);
void Run(View *view);
