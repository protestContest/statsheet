#pragma once
#include "kit.h"

struct PageElement;
typedef void (*PageElementDraw)(struct PageElement *el);
typedef void (*PageElementInput)(struct PageElement *el, u16 input);

typedef struct PageElement {
  Rect bounds;
  bool active;
  PageElementDraw draw;
  PageElementInput onInput;
  struct PageElement *next;
} PageElement;

struct Page;
typedef void (*PageInput)(struct Page *page, u16 input);

typedef struct Page {
  char *title;
  PageElement *elements;
  PageElement *focus;
} Page;

void InitPage(Page *page, char *title);
void AddPageElement(Page *page, PageElement *element);
void DrawPage(Page *page);
void OnPageInput(Page *page, u16 input);
void SelectNextElement(Page *page);
void SelectPrevElement(Page *page);

void InitPageElement(PageElement *el, Rect *bounds, PageElementDraw draw);
