#pragma once
#include "stat.h"
#include "view.h"
#include "views/charge_control.h"

typedef struct {
  View asView;
  char *title;
  Stat *stat;
  Stat *maxStat;
  ChargeControl control;
} ChargeStatView;

void InitChargeStatView(ChargeStatView *view, Rect *bounds, char *title, char *statName, char *maxStatName);
