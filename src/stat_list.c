#include "stat_list.h"
#include "cursor.h"

static void StatListInput(PageElement *el, u16 input);

void InitStatList(StatList *list, Rect *bounds, u32 count, char **stats)
{
  InitPageElement(&list->el, bounds, DrawStatList);
  list->el.onInput = StatListInput;
  list->count = count;
  list->items = Alloc(sizeof(Stat*)*count);
  Assert(list->items);
  list->selected = 0;
  for (u32 i = 0; i < count; i++) {
    if (stats[i][0] == '-') {
      list->items[i] = 0;
      continue;
    }
    Stat *stat = GetStat(stats[i]);
    Assert(stat);
    list->items[i] = stat;
  }
}

void DrawStatList(PageElement *el)
{
  StatList *list = (StatList*)el;
  FontInfo info;
  GetFontInfo(&info);
  SetColor(BLACK);

  Rect bounds = list->el.bounds;
  bounds.bottom = bounds.top + info.ascent + info.descent;
  bounds.left += 8;

  for (u32 i = 0; i < list->count; i++) {
    if (list->items[i]) {
      PrintStat(list->items[i], &bounds);
    }
    OffsetRect(&bounds, 0, info.ascent + info.descent);
  }

  if (list->el.active) {
    PlaceCursor(list->el.bounds.left + 8,
        list->el.bounds.top + info.ascent + list->selected*(info.ascent + info.descent) - 3);
    ShowCursor();
  }
}

static void StatListInput(PageElement *el, u16 input)
{
  StatList *list = (StatList*)el;
  if (KeyPressed(BTN_DOWN)) {
    SelectNext(list);
  } else if (KeyPressed(BTN_UP)) {
    SelectPrev(list);
  } else if (KeyPressed(BTN_SELECT)) {
    EditList(list);
  }
}

void SelectNext(StatList *list)
{
  if (list->selected == list->count-1) return;

  list->selected++;
  if (!list->items[list->selected]) {
    SelectNext(list);
    return;
  }
  if (list->el.active) {
    FontInfo info;
    GetFontInfo(&info);
    PlaceCursor(list->el.bounds.left + 8,
        list->el.bounds.top + info.ascent + list->selected*(info.ascent + info.descent) - 3);
  }
}

void SelectPrev(StatList *list)
{
  if (list->selected == 0) return;

  list->selected--;
  if (!list->items[list->selected]) {
    SelectPrev(list);
    return;
  }
  if (list->el.active) {
    FontInfo info;
    GetFontInfo(&info);
    PlaceCursor(list->el.bounds.left + 8,
        list->el.bounds.top + info.ascent + list->selected*(info.ascent + info.descent) - 3);
  }
}

void EditList(StatList *list)
{
  FontInfo info;
  GetFontInfo(&info);

  HideCursor();
  Rect bounds = list->el.bounds;
  bounds.left += 8;
  bounds.top += list->selected*(info.ascent + info.descent);
  bounds.bottom = bounds.top + info.ascent + info.descent;
  EditStat(list->items[list->selected], &bounds);
  ShowCursor();
  FillRect(&list->el.bounds, WHITE);
  DrawStatList((PageElement*)list);
}
