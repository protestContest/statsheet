#pragma once
#include "view.h"

typedef struct {
  View *view;
  View *up;
  View *down;
  View *left;
  View *right;
} PageElement;

typedef struct {
  View asView;
  char *title;
  PageElement *elements;
  u32 selected;
} Page;

void InitPage(Page *page, char *title);
void PageDraw(View *view);
bool PageInput(View *view, u16 input);
void PageActivate(View *view, bool active);
void AddPageView(Page *page, View *view);
void LinkViewUp(Page *page, View *view, View *up);
void LinkViewDown(Page *page, View *view, View *down);
void LinkViewLeft(Page *page, View *view, View *left);
void LinkViewRight(Page *page, View *view, View *right);
void LinkViewBelow(Page *page, View *above, View *below);
void LinkViewBeside(Page *page, View *left, View *right);
void SelectView(Page *page, View *view);
