#pragma once
#include "kit/canvas.h"

struct View;
typedef void (*ViewDraw)(struct View *view);
typedef bool (*ViewInput)(struct View *view, u16 input);
typedef void (*ViewActivate)(struct View *view, bool active);
typedef void (*ViewDestroy)(struct View *view);

typedef struct View {
  Rect bounds;
  ViewDraw draw;
  ViewInput onInput;
  ViewActivate activate;
  ViewDestroy destroy;
} View;
#define InputView(v,i)    ((v)->onInput && (v)->onInput(v,i))
#define DrawView(v)       ((v)->draw && ((v)->draw(v),0))
#define ActivateView(v,a) ((v)->activate && ((v)->activate(v,a),0))
#define DestroyView(v)    ((v)->destroy && ((v)->destroy(v),0))

void InitView(View *view, Rect *bounds, ViewDraw draw, ViewInput onInput, ViewActivate activate, ViewDestroy destroy);
void Run(View *view);
