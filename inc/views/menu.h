#pragma once
#include "view.h"

typedef struct {
  View asView;
  u32 numOptions;
  char **options;
  u32 selected;
} Menu;

void InitMenu(Menu *menu, Rect *bounds, char **options, u32 numOptions);
