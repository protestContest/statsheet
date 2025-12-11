#pragma once
#include "view.h"

typedef struct PageElement {
  View asView;
  struct PageElement *left;
  struct PageElement *right;
  struct PageElement *up;
  struct PageElement *down;
} PageElement;

void InitPageElement(PageElement *el, Rect *bounds, ViewDraw draw, ViewInput onInput);
void LinkElementBelow(PageElement *above, PageElement *below);
void LinkElementBeside(PageElement *left, PageElement *right);
