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
