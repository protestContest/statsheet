#include "dice_page.h"
#include "elements.h"
#include "ui.h"

#define diceObj   3
#define d6Tile    516
#define d8Tile    532
#define d10Tile   548
#define d12Tile   564
#define d20Tile   580
#define d4Tile    596

static u32 diceTiles[] = {d4Tile, d6Tile, d8Tile, d10Tile, d12Tile, d20Tile};
static u32 diceSides[] = {     4,      6,      8,      10,      12,      20};

typedef struct {
  PageElement el;
  u32 curDie;
} DiceElement;

static void DrawDiceElement(PageElement *el)
{
  PlaceObj(diceObj, el->bounds.left, el->bounds.top);
  SetObjTfm(diceObj, 0);
  SetObjDisplay(diceObj, ObjTfmDbl);
  ShowArrows(&el->bounds);
}

static void RollDice(u32 sides)
{
  u32 n = RandomBetween(0, sides);
  char numStr[8] = {0};
  NumToString(n+1, numStr);
  HideAllObjs();
  Alert(numStr);
}

static bool DiceElementInput(PageElement *el, u16 input)
{
  DiceElement *diceEl = (DiceElement*)el;

  if (KeyPressed(BTN_A)) {
    RollDice(diceSides[diceEl->curDie]);
    SetObjDisplay(diceObj, ObjTfmDbl);
    ShowArrows(&el->bounds);
  } else if (KeyPressed(BTN_LEFT)) {
    diceEl->curDie = (diceEl->curDie > 0) ? diceEl->curDie-1 : ArrayCount(diceTiles)-1;
  } else if (KeyPressed(BTN_RIGHT)) {
    diceEl->curDie = (diceEl->curDie + 1) % ArrayCount(diceTiles);
  }
  if (input & (BTN_LEFT | BTN_RIGHT)) {
    SetObjSprite(diceObj, diceTiles[diceEl->curDie]);
  }
  return false;
}

PageElement *NewDiceElement(Rect *bounds)
{
  DiceElement *el = Alloc(sizeof(DiceElement));
  InitPageElement(&el->el, bounds, DrawDiceElement);
  el->el.onInput = DiceElementInput;
  el->curDie = 5;
  SetObjSprite(diceObj, diceTiles[el->curDie]);
  return (PageElement*)el;
}

void InitDicePage(Page *page)
{
  InitPage(page, "Dice");

  HideObj(diceObj);
  SetObjSize(diceObj, Obj32x32);
  SetTfmScale(0, 0x80, 0x80);

  Rect bounds = {SCREEN_W/2-32, SCREEN_H/2-32, SCREEN_W/2+32, SCREEN_H/2+32};
  PageElement *diceEl = NewDiceElement(&bounds);
  AddPageElement(page, diceEl);
  SelectElement(page, diceEl);

  SetRect(&bounds, SCREEN_W/2-20, SCREEN_H - SCREEN_H/4, SCREEN_W/2+20, SCREEN_H - SCREEN_H/4+16);
  PageElement *btn = NewButton(&bounds, "Roll");
  AddPageElement(page, btn);
}
