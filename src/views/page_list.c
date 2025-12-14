#include "views/page_list.h"
#include "kit/canvas.h"
#include "kit/input.h"
#include "kit/vec.h"
#include "view.h"

static void PageListDraw(View *view)
{
  PageList *list = (PageList*)view;
  if (VecCount(list->pages) == 0) return;
  Page *page = list->pages[list->curPage];
  DrawView(&page->asView);
}

static bool PageListInput(View *view, u16 input)
{
  PageList *list = (PageList*)view;
  if (VecCount(list->pages) == 0) return false;
  Page *page = list->pages[list->curPage];
  if (KeyPressed(BTN_L)) {
    SelectPage(list, list->curPage > 0 ? list->curPage - 1 : VecCount(list->pages) - 1);
  } else if (KeyPressed(BTN_R)) {
    SelectPage(list, list->curPage < VecCount(list->pages)-1 ? list->curPage + 1 : 0);
  } else {
    InputView(&page->asView, input);
  }
  return false;
}

static void PageListActivate(View *view, bool active)
{
  PageList *list = (PageList*)view;
  if (VecCount(list->pages) == 0) return;
  Page *page = list->pages[list->curPage];
  ActivateView(&page->asView, active);
}

void InitPageList(PageList *list)
{
  Rect bounds = {0, 0, SCREEN_W, SCREEN_H};
  InitView(&list->asView, &bounds, PageListDraw, PageListInput, PageListActivate);
  list->pages = 0;
  list->curPage = 0;
}

void AddPage(PageList *list, Page *page)
{
  VecPush(list->pages, page);
}

void SelectPage(PageList *list, u32 pageNum)
{
  ActivateView(&list->pages[list->curPage]->asView, false);
  list->curPage = pageNum;
  ActivateView(&list->pages[list->curPage]->asView, true);
  ClearScreen();
  DrawView(&list->pages[list->curPage]->asView);
}
