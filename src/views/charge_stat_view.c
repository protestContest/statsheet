#include "views/charge_stat_view.h"
#include "kit/input.h"
#include "kit/text.h"
#include "ui.h"
#include "views/num_control.h"

static void DrawChargeStatView(View *view)
{
  ChargeStatView *statView = (ChargeStatView*)view;
  FontInfo info;
  GetFontInfo(&info);

  SetColor(BLACK);
  MoveTo(view->bounds.left, view->bounds.top + info.ascent);
  Print(statView->title);

  DrawView(&statView->control.asView);
}

static bool InputChargeStatView(View *view, u16 input)
{
  ChargeStatView *statView = (ChargeStatView*)view;
  if (KeyPressed(BTN_SELECT)) {
    i32 original = statView->stat->value;
    Run(&statView->control.asView);
    if (KeyPressed(BTN_B)) {
      statView->stat->value = original;
      statView->control.value = original;
      EraseRect(&statView->control.asView.bounds);
      DrawView(&statView->control.asView);
    } else {
      UpdateStat(statView->stat, statView->control.value);
    }
  }
  return false;
}

void InitChargeStatView(ChargeStatView *view, Rect *bounds, char *title, char *statName, char *maxStatName)
{
  FontInfo info;
  GetFontInfo(&info);
  InitView(&view->asView, bounds, DrawChargeStatView, InputChargeStatView, 0, 0);
  view->stat = GetStat(statName);
  view->maxStat = GetStat(maxStatName);
  Rect ctlBounds = {bounds->right - 9*view->maxStat->value - 1, bounds->top + info.ascent - 8, bounds->right, bounds->top + info.ascent + 2};
  InitChargeControl(&view->control, &ctlBounds, view->stat->value, view->maxStat->value);

  view->title = title;
  view->control.value = view->stat->value;
}
