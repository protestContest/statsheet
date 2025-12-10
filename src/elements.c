#include "elements.h"
#include "ui.h"

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

PageElement *NewLabel(Rect *bounds, char *title)
{
  Label *label = Alloc(sizeof(Label));
  InitPageElement(&label->el, bounds, DrawLabel);
  label->title = title;
  return (PageElement*)label;
}

static void DrawButton(PageElement *el)
{
  Label *label = (Label*)el;
  FontInfo info;
  GetFontInfo(&info);

  Rect r = el->bounds;
  SetColor(RGB(55, 55, 55));
  MoveTo(r.left, r.bottom-3);
  LineTo(r.left, r.top+2);
  LineTo(r.left+2, r.top);
  LineTo(r.right-3, r.top);
  LineTo(r.right-1, r.top+2);
  LineTo(r.right-1, r.bottom-3);
  SetColor(BLACK);
  LineTo(r.right-3, r.bottom-1);
  LineTo(r.left+2, r.bottom-1);
  InsetRect(&r, 1, 1);
  SetColor(RGB(134, 134, 134));
  MoveTo(r.left, r.bottom-2);
  LineTo(r.left, r.top+1);
  LineTo(r.left+1, r.top);
  LineTo(r.right-2, r.top);
  SetColor(RGB(55, 55, 55));
  LineTo(r.right-1, r.top+1);
  LineTo(r.right-1, r.bottom-2);
  LineTo(r.right-2, r.bottom-1);
  LineTo(r.left, r.bottom-1);
  InsetRect(&r, 1, 1);
  FillRect(&r, RGB(90, 90, 90));

  MoveTo((el->bounds.right+el->bounds.left)/2 - TextWidth(label->title)/2,
         (el->bounds.bottom+el->bounds.top)/2 - (info.ascent+info.descent)/2 + info.ascent);
  SetColor(WHITE);
  Print(label->title);
}

PageElement *NewButton(Rect *bounds, char *title)
{
  Label *label = Alloc(sizeof(Label));
  InitPageElement(&label->el, bounds, DrawButton);
  label->title = title;
  return (PageElement*)label;
}

static void DrawHPElement(PageElement *el)
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

static bool HPElementInput(PageElement *el, u16 input)
{
  MaxStatElement *statEl = (MaxStatElement*)el;
  if (KeyPressed(BTN_SELECT)) {
    Rect bounds = el->bounds;
    bounds.left  = bounds.right - TextWidth("00/00");
    bounds.right = bounds.left + TextWidth("00");
    i32 newValue = EditNum(statEl->stat->value, &bounds, false);
    if (UpdateStat(statEl->stat, newValue)) {
      return true;
    }

    if (KeyPressed(BTN_SELECT)) {
      bounds = el->bounds;
      bounds.left = bounds.right - TextWidth("00");
      newValue = EditNum(statEl->tmp->value, &bounds, true);
      FillRect(&bounds, BG);
      if (UpdateStat(statEl->tmp, newValue)) {
        return true;
      }
    }
    DrawHPElement(el);
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

static void DrawStatElement(PageElement *el)
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

static bool StatElementInput(PageElement *el, u16 input)
{
  StatElement *statEl = (StatElement*)el;
  if (KeyPressed(BTN_SELECT)) {
    Rect bounds = el->bounds;
    bounds.left = bounds.right - TextWidth("00");
    Stat *stat = statEl->tmp ? statEl->tmp : statEl->stat;
    i32 newValue = EditNum(stat->value, &bounds, statEl->tmp ? true : false);
    if (UpdateStat(stat, newValue)) {
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

static void DrawAbilityElement(PageElement *el)
{
  StatElement *statEl = (StatElement*)el;
  FontInfo info;
  GetFontInfo(&info);

  SetColor(BLACK);
  MoveTo(el->bounds.left, el->bounds.top + info.ascent);
  Print(statEl->title);

  char str[8] = {0};
  NumToString(statEl->stat->value, str);
  MoveTo(el->bounds.right - TextWidth(str) - TextWidth("00") - 8, el->bounds.top + info.ascent);
  Print(str);

  ClearStr(str);
  if (statEl->tmp->value >= 0) {
    str[0] = '+';
    NumToString(statEl->tmp->value, str+1);
  } else {
    NumToString(statEl->tmp->value, str);
  }
  MoveTo(el->bounds.right - TextWidth(str), el->bounds.top + info.ascent);
  Print(str);
}

static bool AbilityElementInput(PageElement *el, u16 input)
{
  StatElement *statEl = (StatElement*)el;
  if (KeyPressed(BTN_SELECT)) {
    Rect bounds = el->bounds;
    bounds.left = bounds.right - 2*TextWidth("00") - 8;
    bounds.right = bounds.left + TextWidth("00");
    i32 newValue = EditNum(statEl->stat->value, &bounds, false);

    if (UpdateStat(statEl->stat, newValue)) {
      return true;
    }
  }
  return false;
}

PageElement *NewAbilityStatElement(Rect *bounds, char *title, char *statName, char *modName)
{
  PageElement *el = NewStatElement(bounds, title, statName, modName);
  el->onInput = AbilityElementInput;
  el->draw = DrawAbilityElement;
  return el;
}

static void DrawChargeElement(PageElement *el)
{
  ChargeElement *chEl = (ChargeElement*)el;
  FontInfo info;
  GetFontInfo(&info);

  SetColor(BLACK);
  MoveTo(el->bounds.left, el->bounds.top + info.ascent);
  Print(chEl->title);

  Rect bounds = el->bounds;
  i32 max = chEl->max->value;
  bounds.left = bounds.right - 9*max - 1;
  bounds.bottom = bounds.top + info.ascent + info.descent;
  bounds.top = bounds.bottom - 10;
  DrawCharges(chEl->stat->value, max, &bounds);
}

static bool ChargeElementInput(PageElement *el, u16 input)
{
  FontInfo info;
  GetFontInfo(&info);
  ChargeElement *chEl = (ChargeElement*)el;
  if (KeyPressed(BTN_SELECT)) {
    Rect bounds = el->bounds;
    i32 max = chEl->max->value;
    bounds.left = bounds.right - 9*max - 1;
    bounds.bottom = bounds.top + info.ascent + info.descent;
    bounds.top = bounds.bottom - 10;
    i32 newValue = EditCharges(chEl->stat->value, max, &bounds);
    if (UpdateStat(chEl->stat, newValue)) {
      return true;
    }
  }
  return false;
}

PageElement *NewChargeElement(Rect *bounds, char *title, char *statName, char *maxStatName)
{
  ChargeElement *el = Alloc(sizeof(ChargeElement));
  InitPageElement(&el->el, bounds, DrawChargeElement);
  el->el.onInput = ChargeElementInput;
  el->title = title;
  el->stat = GetStat(statName);
  Assert(el->stat);
  el->max = GetStat(maxStatName);
  Assert(el->max);
  return (PageElement*)el;
}
