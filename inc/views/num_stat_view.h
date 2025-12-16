#pragma once
#include "stat.h"
#include "view.h"
#include "views/label.h"
#include "views/num_control.h"

typedef struct {
  View asView;
  Stat *stat;
  Stat *editStat;
  Label label;
  NumControl control;
} NumStatView;

void InitNumStatView(NumStatView *view, Rect *bounds, char *title, char *statName, char *editStatName);
