#pragma once
#include "view.h"

typedef struct {
  View asView;
  char *title;
  bool active;
  bool disabled;
} Button;

void InitButton(Button *button, Rect *bounds, char *title);
