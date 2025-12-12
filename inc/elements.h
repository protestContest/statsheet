#pragma once
#include "kit/sprite.h"
#include "page_element.h"
#include "stat.h"

typedef struct {
  union {
    View asView;
    PageElement asElement;
  };
  char *title;
} Label;
PageElement *NewLabel(Rect *bounds, char *title);

typedef struct {
  union {
    View asView;
    PageElement asElement;
  };
  char *title;
  bool active;
} Button;
PageElement *NewButton(Rect *bounds, char *title);

struct StatElement;
typedef bool (*StatEdit)(struct StatElement *stat);
typedef void (*StatAction)(struct StatElement *stat);
typedef struct StatElement {
  union {
    View asView;
    PageElement asElement;
  };
  char *title;
  StatEdit edit;
  StatAction action;
  Stat *stat;
  Stat *tmp;
} StatElement;
void InitStatElement(StatElement *el, Rect *bounds, char *title, char *statName, char *tmpName);
PageElement *NewStatElement(Rect *bounds, char *title, char *statName, char *tmpName);
PageElement *NewAbilityStatElement(Rect *bounds, char *title, char *statName, char *modName);
PageElement *NewModElement(Rect *bounds, char *title, char *statName, char *tmpName);

typedef struct {
  union {
    View asView;
    PageElement asElement;
    StatElement asStatElement;
  };
  Stat *max;
} MaxStatElement;
PageElement *NewHPElement(Rect *bounds, char *title);
PageElement *NewChargeElement(Rect *bounds, char *title, char *statName, char *maxStatName);

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
