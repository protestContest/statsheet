#pragma once
#include "view.h"

typedef struct {
  View asView;
  u16 value;
  u16 max;
} ChargeControl;

void InitChargeControl(ChargeControl *ctl, Rect *bounds, i32 value, i32 max);
