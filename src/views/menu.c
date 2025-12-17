#include "views/menu.h"
#include "kit/canvas.h"
#include "kit/input.h"
#include "kit/text.h"
#include "ui.h"
#include "views/page_list.h"

static void DrawMenu(View *view)
{
  FontInfo info;
  GetFontInfo(&info);
  Menu *menu = (Menu*)view;
  DrawWindow(&view->bounds);

  ShowCursor();

  SetColor(BLACK);
  Rect itemBounds = {view->bounds.left+10, view->bounds.top + 3, view->bounds.right - 3, view->bounds.top + 3 + info.ascent + info.descent};
  for (u32 i = 0; i < menu->numOptions; i++) {
    if (i == menu->selected) {
      PlaceCursor(itemBounds.left, itemBounds.top + (info.ascent+info.descent)/2);
    }
    MoveTo(itemBounds.left, itemBounds.top + info.ascent);
    Print(menu->options[i]);
    OffsetRect(&itemBounds, 0, info.ascent + info.descent);
  }
}

static bool InputMenu(View *view, u16 input)
{
  Menu *menu = (Menu*)view;

  if (KeyPressed(BTN_START) || KeyPressed(BTN_A) || KeyPressed(BTN_B)) {
    return true;
  } else if (KeyPressed(BTN_UP) && menu->selected > 0) {
    menu->selected--;
    DrawView(view);
  } else if (KeyPressed(BTN_DOWN) && menu->selected < menu->numOptions-1) {
    menu->selected++;
    DrawView(view);
  }

  return false;
}

static void ActivateMenu(View *view, bool active)
{
  if (!active) {
    SetBackground(BG);
    RedrawPage();
  }
}

void InitMenu(Menu *menu, Rect *bounds, char **options, u32 numOptions)
{
  InitView(&menu->asView, bounds, DrawMenu, InputMenu, ActivateMenu);
  menu->numOptions = numOptions;
  menu->options = options;
  menu->selected = 0;
}
