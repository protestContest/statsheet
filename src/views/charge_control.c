#include "views/charge_control.h"
#include "kit/input.h"
#include "kit/text.h"
#include "ui.h"


static void DrawChargeControl(View *view)
{
  ChargeControl *ctl = (ChargeControl*)view;
  SetColor(BLACK);
  FrameRect(&view->bounds);
  for (u16 i = 1; i < ctl->max; i++) {
    MoveTo(view->bounds.left + i*9, view->bounds.top);
    Line(0, 9);
  }
  for (u16 i = 0; i < ctl->value; i++) {
    Rect r = {view->bounds.left + i*9+2, view->bounds.top + 2, view->bounds.left + i*9+8, view->bounds.bottom - 2};
    FillRect(&r, BLACK);
  }
}

static bool InputChargeControl(View *view, u16 input)
{
  ChargeControl *ctl = (ChargeControl*)view;

  if (KeyPressed(BTN_A) || KeyPressed(BTN_B)) {
    return true;
  } else if (KeyPressed(BTN_RIGHT) && ctl->value < ctl->max) {
    ctl->value++;
    EraseRect(&view->bounds);
    DrawView(view);
  } else if (KeyPressed(BTN_LEFT) && ctl->value > 0) {
    ctl->value--;
    EraseRect(&view->bounds);
    DrawView(view);
  }
  return false;
}

static void ActivateChargeControl(View *view, bool active)
{
  Rect r = view->bounds;
  InsetRect(&r, -2, -2);

  if (active) {
    SetColor(BLACK);
    FrameRect(&r);
    ShowArrows(&r, dirH);
  } else {
    EraseRect(&r);
    DrawView(view);
    HideArrows();
  }
}

void InitChargeControl(ChargeControl *ctl, Rect *bounds, i32 value, i32 max)
{
  InitView(&ctl->asView, bounds, DrawChargeControl, InputChargeControl, ActivateChargeControl);
  ctl->value = value;
  ctl->max = max;
}

