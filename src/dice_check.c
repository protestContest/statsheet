#include "dice_check.h"
#include "kit/input.h"
#include "kit/interrupt.h"
#include "views/dice_check_view.h"

void DiceCheck(u32 die, i32 mod)
{
  DiceCheckView dcView;
  i16 w = 200;
  i16 h = 100;
  Rect bounds = {SCREEN_W/2-w/2, SCREEN_H/2-h/2, SCREEN_W/2+w/2, SCREEN_H/2+h/2};
  InitDiceCheckView(&dcView, &bounds, die, mod);

  Run(&dcView.asView);
}

void StatDiceCheck(Stat *stat)
{
  DiceCheck(d20, stat->value);
}

