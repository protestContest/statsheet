#include "overview_page.h"
#include "stat.h"
#include "ui.h"

typedef struct {
  PageElement el;
  char *title;
} Label;

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

typedef struct {
  PageElement el;
  char *title;
  Stat *stat;
  Stat *max;
} ChargeElement;

void DrawLabel(PageElement *el)
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

void DrawAbilityElement(PageElement *el)
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

bool AbilityElementInput(PageElement *el, u16 input)
{
  StatElement *statEl = (StatElement*)el;
  if (KeyPressed(BTN_SELECT)) {
    Rect bounds = el->bounds;
    bounds.left = bounds.right - 2*TextWidth("00") - 8;
    bounds.right = bounds.left + TextWidth("00");
    i32 newValue = EditNum(statEl->stat->value, &bounds, false);
    if (newValue != statEl->stat->value) {
      UpdateStat(statEl->stat, newValue);
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

void DrawChargeElement(PageElement *el)
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

bool ChargeElementInput(PageElement *el, u16 input)
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
    if (newValue != chEl->stat->value) {
      UpdateStat(chEl->stat, newValue);
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

void InitOverviewPage(Page *page)
{
  Rect bounds;
  FontInfo info;
  GetFontInfo(&info);
  u32 lineHeight = info.ascent + info.descent;

  InitPage(page, "Overview", 14);

  SetRect(&bounds, 8, 15, SCREEN_W/2-4, 15 + lineHeight);
  PageElement *hpElement = NewHPElement(&bounds, "HP");
  AddPageElement(page, hpElement);
  page->selected = hpElement;

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *acElement = NewStatElement(&bounds, "AC", "AC", "ArmorMod");
  AddPageElement(page, acElement);
  AddElementBelow(hpElement, acElement);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *speedEl = NewStatElement(&bounds, "Speed", "Speed", 0);
  AddPageElement(page, speedEl);
  AddElementBelow(acElement, speedEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *spellSaveEl = NewStatElement(&bounds, "Spell Save DC", "SpellSaveDC", 0);
  AddPageElement(page, spellSaveEl);
  AddElementBelow(speedEl, spellSaveEl);

  OffsetRect(&bounds, 0, lineHeight+8);
  PageElement *strEl = NewAbilityStatElement(&bounds, "STR", "STR", "ModSTR");
  AddPageElement(page, strEl);
  AddElementBelow(spellSaveEl, strEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *dexEl = NewAbilityStatElement(&bounds, "DEX", "DEX", "ModDEX");
  AddPageElement(page, dexEl);
  AddElementBelow(strEl, dexEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *conEl = NewAbilityStatElement(&bounds, "CON", "CON", "ModCON");
  AddPageElement(page, conEl);
  AddElementBelow(dexEl, conEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *intEl = NewAbilityStatElement(&bounds, "INT", "INT", "ModINT");
  AddPageElement(page, intEl);
  AddElementBelow(conEl, intEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *wisEl = NewAbilityStatElement(&bounds, "WIS", "WIS", "ModWIS");
  AddPageElement(page, wisEl);
  AddElementBelow(intEl, wisEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *chaEl = NewAbilityStatElement(&bounds, "CHA", "CHA", "ModCHA");
  AddPageElement(page, chaEl);
  AddElementBelow(wisEl, chaEl);

  SetRect(&bounds, SCREEN_W - SCREEN_W/4 - TextWidth("Spell Slots")/2, 15, SCREEN_W-4, 15 + lineHeight);
  PageElement *slotsLabel = NewLabel(&bounds, "Spell Slots");
  AddPageElement(page, slotsLabel);

  SetRect(&bounds, SCREEN_W/2+10, 15 + lineHeight + 5, SCREEN_W-4, 15+2*lineHeight + 5);
  PageElement *l1slots = NewChargeElement(&bounds, "L1", "L1SlotsUsed", "L1Slots");
  AddPageElement(page, l1slots);
  AddElementBeside(acElement, l1slots);
  hpElement->right = l1slots;

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *l2slots = NewChargeElement(&bounds, "L2", "L2SlotsUsed", "L2Slots");
  AddPageElement(page, l2slots);
  AddElementBelow(l1slots, l2slots);
  AddElementBeside(speedEl, l2slots);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *l3slots = NewChargeElement(&bounds, "L3", "L3SlotsUsed", "L3Slots");
  AddPageElement(page, l3slots);
  AddElementBelow(l2slots, l3slots);
  AddElementBeside(spellSaveEl, l3slots);

  strEl->right = l3slots;
  dexEl->right = l3slots;
  conEl->right = l3slots;
  intEl->right = l3slots;
  wisEl->right = l3slots;
  chaEl->right = l3slots;

  SelectElement(page, hpElement);
}
