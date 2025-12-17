#include "views/num_stat_view.h"
#include "kit/input.h"
#include "kit/observe.h"
#include "ui.h"
#include "views/num_control.h"
#include "kit/text.h"
#include "dice_check.h"

static void DrawNumStatView(View *view)
{
  NumStatView *statView = (NumStatView*)view;
  DrawView(&statView->label.asView);
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
    EraseRect(&statView->control.asView.bounds);
    Run(&statView->control.asView);
    if (KeyPressed(BTN_B)) {
      editStat->value = original;
    } else {
      UpdateStat(editStat, statView->control.value);
    }

    statView->control.value = statView->stat->value;
    statView->control.showSign = wasSignShowing;
    EraseRect(&statView->control.asView.bounds);
    DrawView(&statView->asView);
  } else if (KeyPressed(BTN_A)) {
    StatDiceCheck(statView->label.title, statView->stat);
  }
  return false;
}

static void OnStatChange(View *view, Stat *stat, u32 event)
{
  NumStatView *statView = (NumStatView*)view;
  statView->control.value = stat->value;
  if (statView->active) {
    EraseRect(&view->bounds);
    DrawView(view);
  }
}

static void NumStatViewActivate(View *view, bool active)
{
  NumStatView *statView = (NumStatView*)view;
  statView->active = active;
}

void InitNumStatView(NumStatView *view, Rect *bounds, char *title, char *statName, char *editStatName)
{
  InitView(&view->asView, bounds, DrawNumStatView, InputNumStatView, NumStatViewActivate, 0);

  Rect childBounds = *bounds;
  childBounds.right = childBounds.left + TextWidth(title);
  InitLabel(&view->label, &childBounds, title);
  childBounds = *bounds;
  childBounds.left = childBounds.right - TextWidth("000");
  InitNumControl(&view->control, &childBounds, 0, dirH, false);

  view->stat = GetStat(statName);
  Observe(view->stat, view, (ObserveFn)OnStatChange);
  view->control.value = view->stat->value;
  view->editStat = 0;
  if (editStatName && *editStatName) {
    view->editStat = GetStat(editStatName);
  }
  view->active = false;
}
