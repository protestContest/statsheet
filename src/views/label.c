#include "views/label.h"
#include "kit/text.h"

static void DrawLabel(View *view)
{
  Label *label = (Label*)view;
  FontInfo info;
  GetFontInfo(&info);
  SetColor(BLACK);
  MoveTo(view->bounds.left, view->bounds.top + info.ascent);
  Print(label->title);
}

void InitLabel(Label *label, i16 x, i16 y, char *title)
{
  FontInfo info;
  GetFontInfo(&info);
  Rect bounds = {x, y, x + TextWidth(title), y + info.ascent + info.descent};
  InitView(&label->asView, &bounds, DrawLabel, 0, 0);
  label->title = title;
}
