#pragma once
#include "kit/canvas.h"
#include "view.h"

typedef struct {
  View asView;
  u32 curDie;
  bool selectDir;
} DiceView;

void InitDiceView(DiceView *view, Rect *bounds, u32 die, VHSelect selectDir);
u32 RollDice(u32 die);
