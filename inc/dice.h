#pragma once
#include "page_element.h"

enum {d4, d6, d8, d10, d12, d20};

typedef struct {
  union {
    View asView;
    PageElement asElement;
  };
  u32 curDie;
  bool selectDir;
  AnimatedSprite *sprite;
} DiceElement;
PageElement *NewDiceElement(Rect *bounds, bool selectDir);
u32 RollDice(DiceElement *el);
void DiceCheck(u32 die, i32 mod);
