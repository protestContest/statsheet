#include "page.h"
#include "ui.h"

void InitPage(Page *page, char *title)
{
  Rect bounds = {0, 0, SCREEN_W, SCREEN_H};
  InitView(&page->asView, &bounds, DrawPage, PageInput, ActivatePage);
  page->title = title;
  InitVec(&page->elements, sizeof(PageElement*), 0);
  page->selected = 0;
}

void AddPageElement(Page *page, PageElement *element)
{
  VecPush(&page->elements, &element);
}

void SelectElement(Page *page, PageElement *element)
{
  page->selected = element;
  PlaceCursor(element->asView.bounds.left, element->asView.bounds.top + 7);
  ShowCursor();
}

void DrawPage(View *view)
{
  Page *page = (Page*)view;
  FontInfo info;
  GetFontInfo(&info);
  u32 width = TextWidth(page->title);
  Rect rect = {0, 0, SCREEN_W, info.ascent + info.descent};
  FillRect(&rect, DK_GRAY);
  SetColor(WHITE);
  MoveTo(SCREEN_W/2 - width/2, info.ascent);
  Print(page->title);

  for (u32 i = 0; i < page->elements.count; i++) {
    View **el = VecAt(&page->elements, i);
    (*el)->draw(*el);
  }
}

void PageInput(View *view, u16 input)
{
  Page *page = (Page*)view;
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
    page->selected->asView.onInput(&page->selected->asView, input);
  }
}

void ActivatePage(View *view, bool active)
{
  Page *page = (Page*)view;

  if (active) {
    ClearScreen(BG);
    DrawPage(view);
  } else {
    HideAllObjs();
  }

  if (page->selected) {
    if (page->selected->asView.activate) {
      page->selected->asView.activate(&page->selected->asView, active);
    }
  }
}
