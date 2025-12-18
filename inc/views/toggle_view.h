#pragma once
#include "stat.h"
#include "view.h"
#include "views/toggle_control.h"

typedef struct {
  View asView;
  char *title;
  Stat *stat;
  ToggleControl control;
} ToggleView;

void InitToggleView(ToggleView *view, Rect *bounds, char *title, char *statName);
