#pragma once
#include "view.h"

typedef struct {
  View asView;
  char *title;
} Label;

void InitLabel(Label *label, i16 x, i16 y, char *title);
