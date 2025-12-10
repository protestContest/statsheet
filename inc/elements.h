#pragma once
#include "page.h"
#include "stat.h"

typedef struct {
  PageElement el;
  char *title;
} Label;

typedef struct {
  PageElement el;
  char *title;
  Stat *stat;
  Stat *tmp;
} StatElement;

typedef struct {
  PageElement el;
  char *title;
  Stat *stat;
  Stat *max;
  Stat *tmp;
} MaxStatElement;

typedef struct {
  PageElement el;
  char *title;
  Stat *stat;
  Stat *max;
} ChargeElement;

PageElement *NewLabel(Rect *bounds, char *title);
PageElement *NewButton(Rect *bounds, char *title);
PageElement *NewHPElement(Rect *bounds, char *title);
PageElement *NewStatElement(Rect *bounds, char *title, char *statName, char *tmpName);
PageElement *NewAbilityStatElement(Rect *bounds, char *title, char *statName, char *modName);
PageElement *NewChargeElement(Rect *bounds, char *title, char *statName, char *maxStatName);
