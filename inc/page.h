#pragma once
#include "kit.h"
#include "view.h"
#include "page_element.h"

typedef struct {
  View asView;
  char *title;
  Vec elements;
  PageElement *selected;
} Page;

void InitPage(Page *page, char *title);
void AddPageElement(Page *page, PageElement *element);
void SelectElement(Page *page, PageElement *element);

void DrawPage(View *view);
void PageInput(View *view, u16 input);
void ActivatePage(View *view, bool active);
