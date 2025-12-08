#include "label.h"

static void DrawLabel(PageElement *el);

void InitLabel(Label *label, Rect *bounds, char *title)
{
  InitPageElement(&label->el, bounds, DrawLabel);
  label->title = title;
}

static void DrawLabel(PageElement *el)
{
  Label *label = (Label*)el;
  FontInfo info;
  GetFontInfo(&info);
  MoveTo(el->bounds.left, el->bounds.top + info.ascent);
  SetColor(BLACK);
  Print(label->title);
  MoveTo(el->bounds.left, el->bounds.top + info.ascent + 1);
  Line(TextWidth(label->title), 0);
}
