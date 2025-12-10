#pragma once
#include "kit.h"

struct PageElement;
typedef void (*PageElementDraw)(struct PageElement *el);
typedef bool (*PageElementInput)(struct PageElement *el, u16 input);

typedef struct PageElement {
  Rect bounds;
  PageElementDraw draw;
  PageElementInput onInput;
  struct PageElement *left;
  struct PageElement *right;
  struct PageElement *up;
  struct PageElement *down;
} PageElement;

struct Page;
typedef void (*PageActivate)(struct Page *page, bool active);

typedef struct Page {
  char *title;
  Vec elements;
  PageElement *selected;
  PageActivate activate;
} Page;

void InitPage(Page *page, char *title);
void AddPageElement(Page *page, PageElement *element);
void AddElementBelow(PageElement *above, PageElement *below);
void AddElementBeside(PageElement *left, PageElement *right);
void DrawPage(Page *page);
void OnPageInput(Page *page, u16 input);

void InitPageElement(PageElement *el, Rect *bounds, PageElementDraw draw);
void SelectElement(Page *page, PageElement *element);
void ActivatePage(Page *page, bool active);
