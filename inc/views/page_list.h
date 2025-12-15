#pragma once
#include "view.h"
#include "views/page.h"

typedef struct {
  View asView;
  Page **pages;
  u32 curPage;
} PageList;

void InitPageList(PageList *list);
void AddPage(PageList *list, Page *page);
void SelectPage(PageList *list, u32 pageNum);
PageList *BuildPageList(char *name);
