#pragma once
#include "kit.h"
#include "view.h"
#include "page_element.h"

typedef struct Page {
  View asView;
  char *title;
  Vec elements;
  PageElement *selected;
  struct Page *prev;
  struct Page *next;
} Page;

void InitPage(Page *page, char *title);
void AddPageElement(Page *page, PageElement *element);
PageElement *GetPageElement(Page *page, u32 index);
void SelectElement(Page *page, PageElement *element);
void LinkPage(Page *left, Page *right);

void DrawPage(View *view);
void PageInput(View *view, u16 input);
void ActivatePage(View *view, bool active);

void SelectPage(Page *page);
void NextPage(void);
void PrevPage(void);
void RedrawPage(void);
void ReactivatePage(void);
void HandleInput(u16 input);
