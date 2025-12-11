#include "view.h"

void InitView(View *view, Rect *bounds, ViewDraw draw, ViewInput onInput, ViewActivate activate)
{
  view->bounds = *bounds;
  view->draw = draw;
  view->onInput = onInput;
  view->activate = activate;
}
