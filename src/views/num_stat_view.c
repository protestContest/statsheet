#include "views/num_stat_view.h"
#include "kit/input.h"
#include "kit/str.h"
#include "kit/text.h"
#include "ui.h"
#include "views/num_control.h"

static void DrawNumStatView(View *view)
{
  NumStatView *statView = (NumStatView*)view;
  FontInfo info;
  GetFontInfo(&info);

  SetColor(BLACK);
  MoveTo(view->bounds.left, view->bounds.top + info.ascent);
  Print(statView->title);

  DrawView(&statView->control.asView);
}

static bool InputNumStatView(View *view, u16 input)
{
  NumStatView *statView = (NumStatView*)view;
  if (KeyPressed(BTN_SELECT)) {
    Stat *editStat = statView->editStat ? statView->editStat : statView->stat;
    i32 original = editStat->value;
    bool wasSignShowing = statView->control.showSign;
    statView->control.value = editStat->value;
    statView->control.showSign = statView->editStat ? true : wasSignShowing;
    FillRect(&statView->control.asView.bounds, BG);
    Run(&statView->control.asView);
    if (KeyPressed(BTN_B)) {
      editStat->value = original;
    } else {
      UpdateStat(editStat, statView->control.value);
    }

    statView->control.value = statView->stat->value;
    statView->control.showSign = wasSignShowing;
    FillRect(&statView->control.asView.bounds, BG);
    DrawView(&statView->asView);
  }
  return false;
}

void InitNumStatView(NumStatView *view, Rect *bounds, char *title, char *statName, char *editStatName)
{
  InitView(&view->asView, bounds, DrawNumStatView, InputNumStatView, 0);
  Rect ctlBounds = {bounds->right - TextWidth("+00"), bounds->top, bounds->right, bounds->bottom};
  InitNumControl(&view->control, &ctlBounds, 0, horizontal, false);

  view->stat = GetStat(statName);
  view->title = title;
  view->control.value = view->stat->value;
  view->editStat = 0;
  if (editStatName) {
    view->editStat = GetStat(editStatName);
  }
}
