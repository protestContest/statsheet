#pragma once
#include "kit.h"
#include "page.h"

typedef struct {
  PageElement el;
  char *title;
} Label;

void InitLabel(Label *label, Rect *bounds, char *title);
