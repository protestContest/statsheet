#pragma once
#include "view.h"

typedef struct {
  View **content;
} TabContent;

typedef struct {
  View asView;
  TabContent *tabs;
  u32 selectedTab;
} TabView;

void InitTabView(TabView *view, Rect *bounds);
void AddTabView(TabView *tabs, u32 tabNum, View *view);
void SelectTab(TabView *tabView, i32 tabNum);
View *GetTabView(TabView *tabView, u32 viewNum);
