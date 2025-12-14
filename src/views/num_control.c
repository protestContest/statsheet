#include "views/num_control.h"
#include "kit/canvas.h"
#include "kit/input.h"
#include "kit/str.h"
#include "kit/text.h"
#include "ui.h"

static void DrawNumControl(View *view)
{
  NumControl *ctl = (NumControl*)view;
  FontInfo info;
  GetFontInfo(&info);
  char numStr[8] = {0};
  if (ctl->showSign && ctl->value >= 0) {
    numStr[0] = '+';
    NumToString(ctl->value, numStr+1);
  } else {
    NumToString(ctl->value, numStr);
  }
  SetColor(BLACK);
  MoveTo(view->bounds.right - TextWidth(numStr), view->bounds.top + info.ascent);
  Print(numStr);
}

static bool InputNumControl(View *view, u16 input)
{
  NumControl *ctl = (NumControl*)view;
  FontInfo info;
  GetFontInfo(&info);
  u16 upBtn = ctl->selectDir == vertical ? BTN_UP : BTN_RIGHT;
  u16 downBtn = ctl->selectDir == vertical ? BTN_DOWN : BTN_LEFT;
  if (KeyPressed(BTN_A) || KeyPressed(BTN_B)) {
    return true;
  } else if (KeyPressed(upBtn)) {
    ctl->value++;
    FillRect(&view->bounds, BG);
    DrawView(view);
  } else if (KeyPressed(downBtn)) {
    ctl->value--;
    FillRect(&view->bounds, BG);
    DrawView(view);
  }
  return false;
}

static void ActivateNumControl(View *view, bool active)
{
  NumControl *ctl = (NumControl*)view;
  Rect r = view->bounds;
  InsetRect(&r, -2, -2);

  if (active) {
    SetColor(BLACK);
    FrameRect(&r);
    ShowArrows(&r, ctl->selectDir);
  } else {
    FillRect(&r, BG);
    DrawView(view);
    HideArrows();
  }
}

void InitNumControl(NumControl *ctl, Rect *bounds, i32 value, bool selectDir, bool showSign)
{
  InitView(&ctl->asView, bounds, DrawNumControl, InputNumControl, ActivateNumControl);
  ctl->value = value;
  ctl->selectDir = selectDir;
  ctl->showSign = showSign;
}
