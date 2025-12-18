#include "dice_check.h"
#include "views/dice_check_view.h"

void DiceCheck(char *title, u32 die, i32 mod)
{
  DiceCheckView dcView;
  i16 w = 200;
  i16 h = 100;
  Rect bounds = {SCREEN_W/2-w/2, SCREEN_H/2-h/2, SCREEN_W/2+w/2, SCREEN_H/2+h/2};
  InitDiceCheckView(&dcView, &bounds, title, die, mod);

  Run(&dcView.asView);
}

void StatDiceCheck(char *title, Stat *stat)
{
  DiceCheck(title, d20, stat->value);
}

