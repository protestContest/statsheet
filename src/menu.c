#include "menu.h"
#include "kit/input.h"
#include "ui.h"
#include "page.h"

void InitMenu(Menu *menu)
{
  menu->width = 0;
  menu->height = 0;
  InitVec(&menu->items, sizeof(MenuItem), 0);
}

void AddMenuItem(Menu *menu, char *title, MenuAction action, void *data)
{
  FontInfo info;
  GetFontInfo(&info);
  menu->height += info.ascent + info.descent;
  menu->width = Max(menu->width, TextWidth(title));
  MenuItem item = {title, action, data};
  VecPush(&menu->items, &item);
}

static void SelectItem(Menu *menu, u32 item)
{
  FontInfo info;
  GetFontInfo(&info);
  u32 lineHeight = info.ascent + info.descent;

  PlaceCursor(SCREEN_W/2-menu->width/2 + 2, SCREEN_H/2-menu->height/2 + item*lineHeight + 7);
}

void ShowMenu(Menu *menu)
{
  FontInfo info;
  GetFontInfo(&info);
  u32 lineHeight = info.ascent + info.descent;
  u16 w = menu->width + 16 + 8;
  u16 h = menu->height + 16;
  Rect r = {SCREEN_W/2-w/2, SCREEN_H/2-h/2, SCREEN_W/2+w/2, SCREEN_H/2+h/2};

  HideAllObjs();
  ShowWindow(&r);

  for (u32 i = 0; i < menu->items.count; i++) {
    MenuItem *item = VecAt(&menu->items, i);
    MoveTo(SCREEN_W/2-menu->width/2 + 4, SCREEN_H/2-menu->height/2 + i*lineHeight + info.ascent);
    SetColor(BLACK);
    Print(item->title);
  }

  u32 selected = 0;
  SelectItem(menu, selected);
  ShowCursor();
  bool doIt = false;
  while (true) {
    VSync();
    GetInput();
    if (KeyPressed(BTN_START) || KeyPressed(BTN_B)) {
      break;
    } else if (KeyPressed(BTN_UP) && selected > 0) {
      selected--;
      SelectItem(menu, selected);
    } else if (KeyPressed(BTN_DOWN) && selected < menu->items.count-1) {
      selected++;
      SelectItem(menu, selected);
    } else if (KeyPressed(BTN_A)) {
      doIt = true;
      break;
    }
  }

  HideCursor();
  FillRect(&r, BG);
  RedrawPage();

  if (doIt) {
    MenuItem *item = VecAt(&menu->items, selected);
    Assert(item->action);
    item->action(item->data);
  }

  ReactivatePage();
}
