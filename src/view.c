#include "view.h"
#include "kit/debug.h"
#include "kit/input.h"
#include "kit/interrupt.h"
#include "kit/mem.h"
#include "kit/vec.h"
#include "ui.h"
#include "views/button.h"
#include "views/charge_control.h"
#include "views/charge_stat_view.h"
#include "views/label.h"
#include "views/num_control.h"
#include "views/num_stat_view.h"

static View **runStack = 0;

void InitView(View *view, Rect *bounds, ViewDraw draw, ViewInput onInput, ViewActivate activate)
{
  view->bounds = *bounds;
  view->draw = draw;
  view->onInput = onInput;
  view->activate = activate;
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

View *BuildView (u8 **desc)
{
  Rect bounds;
  View *view;
  i32 a, b;
  char *strA, *strB, *strC;

  u8 type = **desc;
  (*desc)++;
  Copy(*desc, &bounds, sizeof(bounds));
  (*desc) += sizeof(bounds);

  switch (type) {
  case viewButton:
    strA = (char*)*desc;
    while (**desc) (*desc)++;
    (*desc)++;
    view = Alloc(sizeof(Button));
    InitButton((Button*)view, &bounds, strA);
    return view;
  case viewChargeControl:
    Copy(*desc, &a, sizeof(a));
    (*desc) += sizeof(a);
    Copy(*desc, &b, sizeof(b));
    (*desc) += sizeof(b);
    view = Alloc(sizeof(ChargeControl));
    InitChargeControl((ChargeControl*)view, &bounds, a, b);
    return view;
  case viewChargeStat:
    strA = (char*)*desc;
    while (**desc) (*desc)++;
    (*desc)++;
    strB = (char*)*desc;
    while (**desc) (*desc)++;
    (*desc)++;
    strC = (char*)*desc;
    while (**desc) (*desc)++;
    (*desc)++;
    view = Alloc(sizeof(ChargeStatView));
    InitChargeStatView((ChargeStatView*)view, &bounds, strA, strB, strC);
    return view;
  case viewLabel:
    strA = (char*)*desc;
    while (**desc) (*desc)++;
    (*desc)++;
    view = Alloc(sizeof(Label));
    InitLabel((Label*)view, bounds.left, bounds.top, strA);
    return view;
  case viewNumControl:
    Copy(*desc, &a, sizeof(a));
    (*desc) += sizeof(a);
    Copy(*desc, &b, sizeof(b));
    (*desc) += sizeof(b);
    view = Alloc(sizeof(NumControl));
    InitNumControl((NumControl*)view, &bounds, a, b & 1, b & 2);
    return view;
  case viewNumStat:
    strA = (char*)*desc;
    while (**desc) (*desc)++;
    (*desc)++;
    strB = (char*)*desc;
    while (**desc) (*desc)++;
    (*desc)++;
    strC = (char*)*desc;
    while (**desc) (*desc)++;
    (*desc)++;
    view = Alloc(sizeof(NumStatView));
    InitNumStatView((NumStatView*)view, &bounds, strA, strB, strC);
    return view;
  default:
    Error("Bad view description");
  }

  return 0;
}
