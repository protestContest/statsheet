#pragma once
#include "kit.h"

typedef void (*MenuAction)(void *data);

typedef struct {
  char *title;
  MenuAction action;
  void *data;
} MenuItem;

typedef struct {
  u16 width;
  u16 height;
  Vec items;
} Menu;

void InitMenu(Menu *menu);
void AddMenuItem(Menu *menu, char *title, MenuAction action, void *data);
void ShowMenu(Menu *menu);
