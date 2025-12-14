#pragma once
#include "stat.h"
#include "view.h"
#include "views/num_control.h"

typedef struct {
  View asView;
  char *title;
  Stat *stat;
  Stat *editStat;
  NumControl control;
} NumStatView;

void InitNumStatView(NumStatView *view, Rect *bounds, char *title, char *statName, char *editStatName);
