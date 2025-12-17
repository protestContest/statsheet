#include "views/page_list.h"
#include "dice_check.h"
#include "kit/canvas.h"
#include "kit/input.h"
#include "kit/text.h"
#include "kit/vec.h"
#include "prefix.h"
#include "view.h"
#include "save.h"
#include "views/menu.h"

static PageList pages;

static void StartMenu(void)
{
  char *items[] = {
    "Dice Check",
    "Save",
  };
  Menu menu;
  i16 w = 100;
  i16 h = LineHeight()*ArrayCount(items) + 6;
  Rect bounds = {SCREEN_W/2-w/2, SCREEN_H/2-h/2, SCREEN_W/2+w/2, SCREEN_H/2+h/2};

  InitMenu(&menu, &bounds, items, ArrayCount(items));
  Run(&menu.asView);

  if (KeyPressed(BTN_A)) {
    switch (menu.selected) {
    case 0:
      DiceCheck("Dice Check", d20, 0);
      break;
    case 1:
      SaveState();
      break;
    }
  }
}

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
  if (VecCount(list->pages) > 1 && KeyPressed(BTN_L)) {
    SelectPage(list->curPage > 0 ? list->curPage - 1 : VecCount(list->pages) - 1);
  } else if (VecCount(list->pages) > 1 && KeyPressed(BTN_R)) {
    SelectPage(list->curPage < VecCount(list->pages)-1 ? list->curPage + 1 : 0);
  } else if (KeyPressed(BTN_START)) {
    StartMenu();
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

void InitPageList(void)
{
  Rect bounds = {0, 0, SCREEN_W, SCREEN_H};
  InitView(&pages.asView, &bounds, PageListDraw, PageListInput, PageListActivate);
  pages.pages = 0;
  pages.curPage = 0;
}

void AddPage(Page *page)
{
  VecPush(pages.pages, page);
}

void SelectPage(u32 pageNum)
{
  ActivateView(&pages.pages[pages.curPage]->asView, false);
  pages.curPage = pageNum;
  ActivateView(&pages.pages[pages.curPage]->asView, true);
  ClearScreen();
  DrawView(&pages.pages[pages.curPage]->asView);
}

View *GetPageList(void)
{
  return &pages.asView;
}

void RedrawPage(void)
{
  ClearScreen();
  DrawView(GetPageList());
}
