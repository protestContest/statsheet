#pragma once
#include "view.h"

typedef struct {
  View asView;
  bool on;
} ToggleControl;

void InitToggleControl(ToggleControl *ctl, Rect *bounds, bool on);
