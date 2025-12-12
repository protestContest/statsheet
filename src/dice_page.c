#include "dice_page.h"
#include "elements.h"
#include "kit/input.h"
#include "kit/sprite.h"
#include "ui.h"

Page dicePage;

static void DicePageActivate(View *view, bool active)
{
  Page *page = (Page*)view;
  ActivatePage(view, active);
  if (active) {
    DiceElement *dice = (DiceElement*)GetPageElement(page, 0);
    ShowArrows(&dice->asView.bounds, dice->selectDir);
  } else {
    HideArrows();
  }
}

static void DicePageInput(View *view, u16 input)
{
  Page *page = (Page*)view;
  DiceElement *diceEl = *(DiceElement**)VecAt(&page->elements, 0);
  Button *button = *(Button**)VecAt(&page->elements, 1);

  diceEl->asElement.asView.onInput(&diceEl->asElement.asView, input);
  button->asElement.asView.onInput(&button->asElement.asView, input);

  if (KeyReleased(BTN_A)) {
    u32 n = RollDice(diceEl);
    char numStr[8] = {0};
    NumToString(n+1, numStr);

    u32 top = 20;
    u32 wHeight = 30;
    u32 wWidth = 40;
    Rect bounds = {SCREEN_W/2-wWidth/2, top, SCREEN_W/2+wWidth/2, top+wHeight};
    ShowWindow(&bounds);
    FontInfo info;
    GetFontInfo(&info);
    MoveTo(SCREEN_W/2 - TextWidth(numStr)/2, top+wHeight/2 - (info.ascent+info.descent)/2 + info.ascent);
    SetColor(BLACK);
    Print(numStr);
  }
}

void InitDicePage(void)
{
  Page *page = &dicePage;
  InitPage(page, "Dice");
  page->asView.activate = DicePageActivate;
  page->asView.onInput = DicePageInput;

  Rect bounds = {SCREEN_W/2-24, SCREEN_H/2-24, SCREEN_W/2+24, SCREEN_H/2+24};
  PageElement *diceEl = NewDiceElement(&bounds, true);
  AddPageElement(page, diceEl);
  SelectElement(page, diceEl);

  SetRect(&bounds, SCREEN_W/2-20, SCREEN_H - SCREEN_H/4, SCREEN_W/2+20, SCREEN_H - SCREEN_H/4+16);
  PageElement *btn = NewButton(&bounds, "Roll");
  AddPageElement(page, btn);
}
