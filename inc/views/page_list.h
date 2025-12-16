#pragma once
#include "view.h"
#include "views/page.h"

typedef struct {
  View asView;
  Page **pages;
  u32 curPage;
} PageList;

void InitPageList(void);
void AddPage(Page *page);
void SelectPage(u32 pageNum);
View *GetPageList(void);
void RedrawPage(void);
