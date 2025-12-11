#include "elements.h"
#include "ui.h"

static void DrawLabel(View *el)
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
  InitPageElement(&label->asElement, bounds, DrawLabel, 0);
  label->title = title;
  return (PageElement*)label;
}

static void DrawButtonUp(Rect *bounds)
{
  Rect r = *bounds;
  SetColor(RGB(48, 48, 48));
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
  SetColor(RGB(128, 128, 128));
  MoveTo(r.left, r.bottom-2);
  LineTo(r.left, r.top+1);
  LineTo(r.left+1, r.top);
  LineTo(r.right-2, r.top);
  SetColor(RGB(48, 48, 48));
  LineTo(r.right-1, r.top+1);
  LineTo(r.right-1, r.bottom-2);
  LineTo(r.right-2, r.bottom-1);
  LineTo(r.left, r.bottom-1);
  InsetRect(&r, 1, 1);
  FillRect(&r, RGB(90, 90, 90));
}

static void DrawButtonDown(Rect *bounds)
{
  Rect r = *bounds;
  SetColor(RGB(48, 48, 48));
  MoveTo(r.left, r.bottom-3);
  LineTo(r.left, r.top+2);
  LineTo(r.left+2, r.top);
  LineTo(r.right-3, r.top);
  LineTo(r.right-1, r.top+2);
  LineTo(r.right-1, r.bottom-2);
  LineTo(r.right-3, r.bottom-1);
  LineTo(r.left+2, r.bottom-1);
  SetColor(BLACK);
  MoveTo(r.left+1, r.bottom-2);
  LineTo(r.left+1, r.top+2);
  LineTo(r.left+2, r.top+1);
  LineTo(r.right-3, r.top+1);
  SetColor(RGB(128, 128, 128));
  MoveTo(r.right-2, r.top+2);
  LineTo(r.right-2, r.bottom-3);
  LineTo(r.right-3, r.bottom-2);
  LineTo(r.left+2, r.bottom-2);
  InsetRect(&r, 2, 2);
  FillRect(&r, RGB(90, 90, 90));
}

static void DrawButton(View *el)
{
  Button *button = (Button*)el;
  if (button->active) {
    DrawButtonDown(&el->bounds);
  } else {
    DrawButtonUp(&el->bounds);
  }

  FontInfo info;
  GetFontInfo(&info);
  MoveTo((el->bounds.right+el->bounds.left)/2 - TextWidth(button->title)/2,
         (el->bounds.bottom+el->bounds.top)/2 - (info.ascent+info.descent)/2 + info.ascent);
  SetColor(WHITE);
  Print(button->title);
}

void ButtonInput(View *el, u16 input)
{
  Button *button = (Button*)el;
  button->active = (input & BTN_A) ? true : false;

  if (KeyPressed(BTN_A) || KeyReleased(BTN_A)) {
    FillRect(&el->bounds, BG);
    DrawButton(el);
  }
}

PageElement *NewButton(Rect *bounds, char *title)
{
  Button *button = Alloc(sizeof(Button));
  InitPageElement(&button->asElement, bounds, DrawButton, ButtonInput);
  button->title = title;
  button->active = false;
  return (PageElement*)button;
}

static void DrawHPElement(View *el)
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

static void HPElementInput(View *el, u16 input)
{
  MaxStatElement *statEl = (MaxStatElement*)el;
  if (KeyPressed(BTN_SELECT)) {
    Rect bounds = el->bounds;
    bounds.left  = bounds.right - TextWidth("00/00");
    bounds.right = bounds.left + TextWidth("00");
    i32 newValue = EditNum(statEl->stat->value, &bounds, false);
    if (UpdateStat(statEl->stat, newValue)) {
      // todo: redraw page
      return;
    }

    if (KeyPressed(BTN_SELECT)) {
      bounds = el->bounds;
      bounds.left = bounds.right - TextWidth("00");
      newValue = EditNum(statEl->tmp->value, &bounds, true);
      FillRect(&bounds, BG);
      if (UpdateStat(statEl->tmp, newValue)) {
        // todo: redraw apge
        return;
      }
    }
    DrawHPElement(el);
  }
}

PageElement *NewHPElement(Rect *bounds, char *title)
{
  MaxStatElement *hpEl = Alloc(sizeof(MaxStatElement));
  InitPageElement(&hpEl->asElement, bounds, DrawHPElement, HPElementInput);
  hpEl->title = title;
  hpEl->stat = GetStat("HP");
  Assert(hpEl->stat);
  hpEl->max = GetStat("MaxHP");
  Assert(hpEl->max);
  hpEl->tmp = GetStat("TmpMaxHP");
  Assert(hpEl->tmp);
  return (PageElement*)hpEl;
}

static void DrawStatElement(View *el)
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

static void StatElementInput(View *el, u16 input)
{
  StatElement *statEl = (StatElement*)el;
  if (KeyPressed(BTN_SELECT)) {
    Rect bounds = el->bounds;
    bounds.left = bounds.right - TextWidth("00");
    Stat *stat = statEl->tmp ? statEl->tmp : statEl->stat;
    i32 newValue = EditNum(stat->value, &bounds, statEl->tmp ? true : false);
    if (UpdateStat(stat, newValue)) {
      // todo: redraw
    }
  }
}

PageElement *NewStatElement(Rect *bounds, char *title, char *statName, char *tmpName)
{
  StatElement *el = Alloc(sizeof(StatElement));
  InitPageElement(&el->asElement, bounds, DrawStatElement, StatElementInput);
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

static void DrawAbilityElement(View *el)
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

static void AbilityElementInput(View *el, u16 input)
{
  StatElement *statEl = (StatElement*)el;
  if (KeyPressed(BTN_SELECT)) {
    Rect bounds = el->bounds;
    bounds.left = bounds.right - 2*TextWidth("00") - 8;
    bounds.right = bounds.left + TextWidth("00");
    i32 newValue = EditNum(statEl->stat->value, &bounds, false);

    if (UpdateStat(statEl->stat, newValue)) {
      // todo: redraw page
    }
  }
}

PageElement *NewAbilityStatElement(Rect *bounds, char *title, char *statName, char *modName)
{
  PageElement *el = NewStatElement(bounds, title, statName, modName);
  el->asView.onInput = AbilityElementInput;
  el->asView.draw = DrawAbilityElement;
  return el;
}

static void DrawChargeElement(View *el)
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

static void ChargeElementInput(View *el, u16 input)
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
      // todo: redraw page
    }
  }
}

PageElement *NewChargeElement(Rect *bounds, char *title, char *statName, char *maxStatName)
{
  ChargeElement *el = Alloc(sizeof(ChargeElement));
  InitPageElement(&el->asElement, bounds, DrawChargeElement, ChargeElementInput);
  el->title = title;
  el->stat = GetStat(statName);
  Assert(el->stat);
  el->max = GetStat(maxStatName);
  Assert(el->max);
  return (PageElement*)el;
}

#define diceObj   3
#define numDice   6
static u32 diceSides[] = {     4,      6,      8,      10,      12,      20};
static char *diceImgs[] = {
  "d4.tga",
  "d6.tga",
  "d8.tga",
  "d10.tga",
  "d12.tga",
  "d20.tga"
};
static void LoadDice(u32 index, AnimatedSprite *sprite)
{
  TGA *tiles = ResData(GetResource(diceImgs[index]));

  SetPalette(1, (u8*)tiles->data, tiles->paletteSize, tiles->paletteDepth);
  SetObjPalette(diceObj, 1);

  Rect r = {0, 0, 64, 48};
  LoadTiles(tiles, &r, sprite->frames[0].baseTile);
  OffsetRect(&r, 64, 0);
  LoadTiles(tiles, &r, sprite->frames[1].baseTile);
  OffsetRect(&r, 64, 0);
  LoadTiles(tiles, &r, sprite->frames[2].baseTile);
  OffsetRect(&r, 64, 0);
  LoadTiles(tiles, &r, sprite->frames[3].baseTile);
  AssignSprite(diceObj, sprite);
  PlaceObj(diceObj, SCREEN_W/2-48/2, SCREEN_H/2-48/2);
}

static void DrawDiceElement(View *el)
{
  ShowObj(diceObj);
  ShowArrows(&el->bounds);
}

static void DiceElementInput(View *el, u16 input)
{
  DiceElement *diceEl = (DiceElement*)el;

  if (KeyPressed(BTN_LEFT)) {
    diceEl->curDie = (diceEl->curDie > 0) ? diceEl->curDie-1 : numDice-1;
  } else if (KeyPressed(BTN_RIGHT)) {
    diceEl->curDie = (diceEl->curDie + 1) % numDice;
  }
  if (input & (BTN_LEFT | BTN_RIGHT)) {
    LoadDice(diceEl->curDie, diceEl->sprite);
  }
}

PageElement *NewDiceElement(Rect *bounds, AnimatedSprite *sprite)
{
  DiceElement *el = Alloc(sizeof(DiceElement));
  InitPageElement(&el->asElement, bounds, DrawDiceElement, DiceElementInput);
  el->curDie = 5;
  el->sprite = sprite;
  LoadDice(el->curDie, sprite);
  return (PageElement*)el;
}

u32 RollDice(DiceElement *el)
{
  u32 end = TickCount() + 30;
  while (TickCount() < end) {
    UpdateSprite(diceObj);
  }
  SetSpriteFrame(diceObj, 0);

  return RandomBetween(0, diceSides[el->curDie]);
}
