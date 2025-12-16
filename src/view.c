#include "view.h"
#include "kit/input.h"
#include "kit/interrupt.h"
#include "kit/vec.h"
#include "ui.h"

static View **runStack = 0;

void InitView(View *view, Rect *bounds, ViewDraw draw, ViewInput onInput, ViewActivate activate)
{
  view->bounds = *bounds;
  view->draw = draw;
  view->onInput = onInput;
  view->activate = activate;
  view->parent = 0;
}

void Run(View *view)
{
  u16 input;
  HideAllObjs();
  if (VecCount(runStack) > 0) {
    ActivateView(runStack[VecCount(runStack)-1], false);
  }
  VecPush(runStack, view);
  ActivateView(view, true);
  DrawView(view);
  do {
    VSync();
    input = GetInput();
  } while (!InputView(view, input));
  ActivateView(view, false);
  VecPop(runStack);
    if (VecCount(runStack) > 0) {
    ActivateView(runStack[VecCount(runStack)-1], true);
  }
}
