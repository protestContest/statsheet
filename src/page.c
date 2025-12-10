#include "page.h"
#include "ui.h"

void InitPage(Page *page, char *title)
{
  page->title = title;
  InitVec(&page->elements, sizeof(PageElement*), 0);
  page->selected = 0;
  page->activate = 0;
}

void AddPageElement(Page *page, PageElement *element)
{
  VecPush(&page->elements, &element);
}

void AddElementBelow(PageElement *above, PageElement *below)
{
  if (above->down) {
    above->down->up = below;
    below->down = above->down;
  }
  above->down = below;
  below->up = above;
}

void AddElementBeside(PageElement *left, PageElement *right)
{
  if (left->right) {
    left->right->left = right;
    right->right = left->right;
  }
  left->right = right;
  right->left = left;
}

void DrawPage(Page *page)
{
  FontInfo info;
  GetFontInfo(&info);
  u32 width = TextWidth(page->title);
  Rect rect = {0, 0, SCREEN_W, info.ascent + info.descent};
  FillRect(&rect, DK_GRAY);
  SetColor(WHITE);
  MoveTo(SCREEN_W/2 - width/2, info.ascent);
  Print(page->title);

  for (u32 i = 0; i < page->elements.count; i++) {
    PageElement **el = VecAt(&page->elements, i);
    (*el)->draw(*el);
  }
}

void OnPageInput(Page *page, u16 input)
{
  if (!page->selected) return;

  if (KeyPressed(BTN_LEFT) && page->selected->left) {
    SelectElement(page, page->selected->left);
  } else if (KeyPressed(BTN_RIGHT) && page->selected->right) {
    SelectElement(page, page->selected->right);
  } else if (KeyPressed(BTN_UP) && page->selected->up) {
    SelectElement(page, page->selected->up);
  } else if (KeyPressed(BTN_DOWN) && page->selected->down) {
    SelectElement(page, page->selected->down);
  } else {
    Assert(page->selected->onInput);
    if (page->selected->onInput(page->selected, input)) {
      ClearScreen(BG);
      DrawPage(page);
    }
  }
}

void SelectElement(Page *page, PageElement *element)
{
  page->selected = element;
  PlaceCursor(element->bounds.left, element->bounds.top + 7);
  ShowCursor();
}

void InitPageElement(PageElement *el, Rect *bounds, PageElementDraw draw)
{
  el->bounds = *bounds;
  el->draw = draw;
  el->onInput = 0;
  el->left = 0;
  el->right = 0;
  el->up = 0;
  el->down = 0;
}

void ActivatePage(Page *page, bool active)
{
  if (page->activate) {
    page->activate(page, active);
  }
  if (active) {
    ClearScreen(BG);
    DrawPage(page);
  } else {
    HideAllObjs();
  }
}
