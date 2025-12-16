#pragma once
#include "view.h"

typedef struct {
  View asView;
  char *title;
} Label;

void InitLabel(Label *label, Rect *bounds, char *title);
