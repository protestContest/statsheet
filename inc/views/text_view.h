#pragma once
#include "view.h"

typedef struct {
  View asView;
  char *text;
  i16 *lineStarts;
  i16 *pageStarts;
  u32 curPage;
} TextView;

void InitTextView(TextView *view, Rect *bounds, char *text);
