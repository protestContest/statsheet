#include "views/toggle_view.h"
#include "kit/input.h"
#include "kit/text.h"
#include "ui.h"
#include "views/num_control.h"

static void DrawToggleView(View *view)
{
  ToggleView *statView = (ToggleView*)view;
  FontInfo info;
  GetFontInfo(&info);

  SetColor(BLACK);
  MoveTo(view->bounds.left, view->bounds.top + info.ascent);
  Print(statView->title);

  DrawView(&statView->control.asView);
}

static bool InputToggleView(View *view, u16 input)
{
  ToggleView *statView = (ToggleView*)view;
  InputView(&statView->control.asView, input);
  return false;
}

void InitToggleView(ToggleView *view, Rect *bounds, char *title, char *statName)
{
  FontInfo info;
  GetFontInfo(&info);
  InitView(&view->asView, bounds, DrawToggleView, InputToggleView, 0, 0);
  view->stat = GetStat(statName);
  Rect ctlBounds = {bounds->right - 10, bounds->top + info.ascent - 8, bounds->right, bounds->top + info.ascent + 2};
  InitToggleControl(&view->control, &ctlBounds, view->stat->value);
  view->title = title;
}
