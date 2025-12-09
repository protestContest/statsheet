#include "overview_page.h"
#include "stat.h"
#include "ui.h"

typedef struct {
  PageElement el;
  char *title;
  Stat *stat;
  Stat *tmp;
} StatElement;

typedef struct {
  PageElement el;
  char *title;
  Stat *stat;
  Stat *max;
  Stat *tmp;
} MaxStatElement;

void DrawHPElement(PageElement *el)
{
  MaxStatElement *hpEl = (MaxStatElement*)el;
  FontInfo info;
  GetFontInfo(&info);

  SetColor(BLACK);
  MoveTo(el->bounds.left, el->bounds.top + info.ascent);
  Print(hpEl->title);

  char str[16] = {0};
  char *s = NumToString(hpEl->stat->value, str);
  s[0] = '/';
  NumToString(hpEl->max->value + hpEl->tmp->value, s+1);
  MoveTo(el->bounds.right - TextWidth(str), el->bounds.top + info.ascent);
  Print(str);
}

bool HPElementInput(PageElement *el, u16 input)
{
  MaxStatElement *statEl = (MaxStatElement*)el;
  if (KeyPressed(BTN_SELECT)) {
    Rect bounds = el->bounds;
    bounds.left  = bounds.right - TextWidth("00/00");
    bounds.right = bounds.left + TextWidth("00");
    i32 newValue = EditNum(statEl->stat->value, &bounds, false);
    if (newValue != statEl->stat->value) {
      UpdateStat(statEl->stat, newValue);
      return true;
    } else if (KeyPressed(BTN_SELECT)) {
      bounds = el->bounds;
      bounds.left = bounds.right - TextWidth("00");
      newValue = EditNum(statEl->tmp->value, &bounds, true);
      if (newValue != statEl->stat->value) {
        UpdateStat(statEl->tmp, newValue);
        return true;
      }
    }
  }
  return false;
}

PageElement *NewHPElement(Rect *bounds, char *title)
{
  MaxStatElement *hpEl = Alloc(sizeof(MaxStatElement));
  InitPageElement(&hpEl->el, bounds, DrawHPElement);
  hpEl->el.onInput = HPElementInput;
  hpEl->title = title;
  hpEl->stat = GetStat("HP");
  Assert(hpEl->stat);
  hpEl->max = GetStat("MaxHP");
  Assert(hpEl->max);
  hpEl->tmp = GetStat("TmpMaxHP");
  Assert(hpEl->tmp);
  return (PageElement*)hpEl;
}

void DrawStatElement(PageElement *el)
{
  StatElement *statEl = (StatElement*)el;
  FontInfo info;
  GetFontInfo(&info);

  SetColor(BLACK);
  MoveTo(el->bounds.left, el->bounds.top + info.ascent);
  Print(statEl->title);

  i32 tmp = statEl->tmp ? statEl->tmp->value : 0;
  char str[8] = {0};
  NumToString(statEl->stat->value + tmp, str);
  MoveTo(el->bounds.right - TextWidth(str), el->bounds.top + info.ascent);
  Print(str);
}

bool StatElementInput(PageElement *el, u16 input)
{
  StatElement *statEl = (StatElement*)el;
  if (KeyPressed(BTN_SELECT)) {
    Rect bounds = el->bounds;
    bounds.left = bounds.right - TextWidth("00");
    Stat *stat = statEl->tmp ? statEl->tmp : statEl->stat;
    i32 newValue = EditNum(stat->value, &bounds, statEl->tmp ? true : false);
    if (newValue != stat->value) {
      UpdateStat(stat, newValue);
      return true;
    }
  }
  return false;
}

PageElement *NewStatElement(Rect *bounds, char *title, char *statName, char *tmpName)
{
  StatElement *el = Alloc(sizeof(StatElement));
  InitPageElement(&el->el, bounds, DrawStatElement);
  el->el.onInput = StatElementInput;
  el->title = title;
  el->stat = GetStat(statName);
  Assert(el->stat);
  if (tmpName) {
    el->tmp = GetStat(tmpName);
    Assert(el->tmp);
  } else {
    el->tmp = 0;
  }
  return (PageElement*)el;
}

void InitOverviewPage(Page *page)
{
  Rect bounds;
  FontInfo info;
  GetFontInfo(&info);
  u32 lineHeight = info.ascent + info.descent;

  InitPage(page, "Overview", 3);

  SetRect(&bounds, 8, 15, SCREEN_W/2-4, 26);
  PageElement *hpElement = NewHPElement(&bounds, "HP");
  AddPageElement(page, hpElement);
  page->selected = hpElement;

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *acElement = NewStatElement(&bounds, "AC", "AC", "ArmorMod");
  AddPageElement(page, acElement);
  AddElementBelow(hpElement, acElement);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *levelEl = NewStatElement(&bounds, "Level", "Level", 0);
  AddPageElement(page, levelEl);
  AddElementBelow(acElement, levelEl);

  SelectElement(page, hpElement);
}
