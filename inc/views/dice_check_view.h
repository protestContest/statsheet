#pragma once
#include "view.h"
#include "views/dice_view.h"
#include "views/num_control.h"
#include "views/button.h"

typedef struct {
  View asView;
  char *title;
  View *selected;
  bool rolled;
  i32 result;
  DiceView dice;
  NumControl modCtl;
  Button btn;
} DiceCheckView;

void InitDiceCheckView(DiceCheckView *view, Rect *bounds, char *title, u32 die, i32 mod);
