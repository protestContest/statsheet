#pragma once
#include "view.h"

typedef struct {
  View asView;
  i32 value;
  bool selectDir;
  bool showSign;
} NumControl;

void InitNumControl(NumControl *ctl, Rect *bounds, i32 value, bool selectDir, bool showSign);
