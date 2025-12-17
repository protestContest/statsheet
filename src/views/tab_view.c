#include "views/tab_view.h"
#include "kit/canvas.h"
#include "kit/input.h"
#include "kit/vec.h"

static void DrawTabView(View *view)
{
  TabView *tabView = (TabView*)view;
  TabContent *tab = &tabView->tabs[tabView->selectedTab];
  for (u32 i = 0; i < VecCount(tab->content); i++) {
    DrawView(tab->content[i]);
  }

  i16 indWidth = (VecCount(tabView->tabs)-1)*10 + 3;
  Rect indRect = {SCREEN_W/2 - indWidth/2, SCREEN_H - 7, SCREEN_W/2 - indWidth/2+3, SCREEN_H - 4};
  SetColor(BLACK);
  for (u32 i = 0; i < VecCount(tabView->tabs); i++) {
    if (i == tabView->selectedTab) {
      FillRect(&indRect, BLACK);
    } else {
      FrameRect(&indRect);
    }
    OffsetRect(&indRect, 10, 0);
  }
}

static bool TabViewInput(View *view, u16 input)
{
  TabView *tabView = (TabView*)view;
  if (KeyPressed(BTN_LEFT)) {
    SelectTab(tabView, tabView->selectedTab-1);
  } else if (KeyPressed(BTN_RIGHT)) {
    SelectTab(tabView, tabView->selectedTab+1);
  }
  return false;
}

void InitTabView(TabView *view, Rect *bounds)
{
  InitView(&view->asView, bounds, DrawTabView, TabViewInput, 0);
  view->tabs = 0;
  TabContent content = {0};
  VecPush(view->tabs, content);
  view->selectedTab = 0;
}

void AddTabView(TabView *tabView, u32 tabNum, View *view)
{
  for (u32 i = VecCount(tabView->tabs); i <= tabNum; i++) {
    TabContent content = {0};
    VecPush(tabView->tabs, content);
  }

  VecPush(tabView->tabs[tabNum].content, view);
}

void SelectTab(TabView *tabView, i32 tabNum)
{
  if (tabNum < 0 || tabNum >= (i32)VecCount(tabView->tabs)) return;
  tabView->selectedTab = tabNum;
  EraseRect(&tabView->asView.bounds);
  DrawTabView(&tabView->asView);
}

View *GetTabView(TabView *tabView, u32 viewNum)
{
  return tabView->tabs[tabView->selectedTab].content[viewNum];
}
