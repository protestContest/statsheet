#pragma once
#include "view.h"

#define BTN_WIDTH   40
#define BTN_HEIGHT  16

typedef struct {
  View asView;
  char *title;
  bool active;
  bool disabled;
} Button;

void InitButton(Button *button, Rect *bounds, char *title);
