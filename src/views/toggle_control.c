#include "views/toggle_control.h"
#include "kit/input.h"
#include "kit/text.h"
#include "ui.h"


static void DrawToggleControl(View *view)
{
  ToggleControl *ctl = (ToggleControl*)view;
  SetColor(BLACK);
  FrameRect(&view->bounds);
  if (ctl->on) {
    Rect r = {view->bounds.left + 2, view->bounds.top + 2, view->bounds.left + 8, view->bounds.bottom - 2};
    FillRect(&r, BLACK);
  }
}

static bool InputToggleControl(View *view, u16 input)
{
  ToggleControl *ctl = (ToggleControl*)view;

  if (KeyPressed(BTN_A) || KeyPressed(BTN_B)) {
    return true;
  } else if (KeyPressed(BTN_SELECT)) {
    ctl->on = !ctl->on;
    EraseRect(&view->bounds);
    DrawView(view);
  }
  return false;
}

void InitToggleControl(ToggleControl *ctl, Rect *bounds, bool on)
{
  InitView(&ctl->asView, bounds, DrawToggleControl, InputToggleControl, 0, 0);
  ctl->on = on;
}

