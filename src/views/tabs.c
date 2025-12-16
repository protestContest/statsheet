#include "views/tabs.h"
#include "kit/canvas.h"
#include "kit/input.h"
#include "kit/vec.h"
#include "ui.h"

static void DrawTabView(View *view)
{
  TabView *tabView = (TabView*)view;
  TabContent *tab = &tabView->tabs[tabView->selectedTab];
  for (u32 i = 0; i < VecCount(tab->content); i++) {
    DrawView(tab->content[i]);
  }
}

static bool TabViewInput(View *view, u16 input)
{
  TabView *tabView = (TabView*)view;
  if (KeyPressed(BTN_LEFT) && tabView->selectedTab > 0) {
    tabView->selectedTab--;
    FillRect(&view->bounds, BG);
    DrawTabView(view);
  } else if (KeyPressed(BTN_RIGHT) && tabView->selectedTab < VecCount(tabView->tabs)-1) {
    tabView->selectedTab++;
    FillRect(&view->bounds, BG);
    DrawTabView(view);
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
  view->parent = &tabView->asView;
  VecPush(tabView->tabs[tabNum].content, view);
}
