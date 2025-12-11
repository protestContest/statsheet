#pragma once
#include "kit/sprite.h"
#include "page_element.h"
#include "stat.h"

typedef struct {
  PageElement asElement;
  char *title;
} Label;
PageElement *NewLabel(Rect *bounds, char *title);

typedef struct {
  PageElement asElement;
  char *title;
  bool active;
} Button;
PageElement *NewButton(Rect *bounds, char *title);

typedef struct {
  PageElement asElement;
  char *title;
  Stat *stat;
  Stat *tmp;
} StatElement;
PageElement *NewStatElement(Rect *bounds, char *title, char *statName, char *tmpName);
PageElement *NewAbilityStatElement(Rect *bounds, char *title, char *statName, char *modName);
PageElement *NewModElement(Rect *bounds, char *title, char *statName, char *tmpName);

typedef struct {
  PageElement asElement;
  char *title;
  Stat *stat;
  Stat *max;
  Stat *tmp;
} MaxStatElement;
PageElement *NewHPElement(Rect *bounds, char *title);

typedef struct {
  PageElement asElement;
  char *title;
  Stat *stat;
  Stat *max;
} ChargeElement;
PageElement *NewChargeElement(Rect *bounds, char *title, char *statName, char *maxStatName);

typedef struct {
  PageElement asElement;
  u32 curDie;
  AnimatedSprite *sprite;
} DiceElement;
PageElement *NewDiceElement(Rect *bounds, AnimatedSprite *sprite);
u32 RollDice(DiceElement *el);
