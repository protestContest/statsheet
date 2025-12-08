#pragma once
#include "stat.h"
#include "kit.h"
#include "page.h"

typedef struct {
  PageElement el;
  u32 count;
  Stat **items;
  u32 selected;
} StatList;

void InitStatList(StatList *list, Rect *bounds, u32 count, char **stats);
void SelectNext(StatList *list);
void SelectPrev(StatList *list);
void EditList(StatList *list);

void DrawStatList(PageElement *el);
