#include "views/page.h"
#include "kit/canvas.h"
#include "kit/input.h"
#include "kit/text.h"
#include "kit/vec.h"
#include "ui.h"
#include "view.h"

void InitPage(Page *page, char *title)
{
  Rect bounds = {0, 0, SCREEN_W, SCREEN_H};
  InitView(&page->asView, &bounds, PageDraw, PageInput, PageActivate);
  page->title = title;
  page->elements = 0;
  page->views = 0;
  page->selected = 0;
}

void PageDraw(View *view)
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

  for (u32 i = 0; i < VecCount(page->views); i++) {
    DrawView(page->views[i]);
  }
}

bool PageInput(View *view, u16 input)
{
  Page *page = (Page*)view;
  PageElement *selected = VecAt(page->elements, page->selected);
  if (!selected) return false;

  if (KeyPressed(BTN_LEFT) && selected->left) {
    SelectView(page, selected->left);
  } else if (KeyPressed(BTN_RIGHT) && selected->right) {
    SelectView(page, selected->right);
  } else if (KeyPressed(BTN_UP) && selected->top) {
    SelectView(page, selected->top);
  } else if (KeyPressed(BTN_DOWN) && selected->bottom) {
    SelectView(page, selected->bottom);
  } else {
    InputView(selected->view, input);
  }
  return false;
}

void PageActivate(View *view, bool active)
{
  Page *page = (Page*)view;
  PageElement *selected = VecAt(page->elements, page->selected);
  if (active && selected) {
    SelectView(page, selected->view);
  } else {
    HideCursor();
  }
  for (u32 i = 0; i < VecCount(page->views); i++) {
    ActivateView(page->views[i], active);
  }
}

void AddPageView(Page *page, View *view)
{
  VecPush(page->views, view);
}

static PageElement *GetPageElement(Page *page, View *view)
{
  for (u32 i = 0; i < VecCount(page->elements); i++) {
    if (page->elements[i].view == view) return &page->elements[i];
  }
  PageElement el = {view, 0, 0, 0, 0};
  VecPush(page->elements, el);
  return &page->elements[VecCount(page->elements)-1];
}

void LinkViewUp(Page *page, View *view, View *up)
{
  PageElement *el = GetPageElement(page, view);
  el->top = GetPageElement(page, up)->view;
}

void LinkViewDown(Page *page, View *view, View *down)
{
  PageElement *el = GetPageElement(page, view);
  el->bottom = GetPageElement(page, down)->view;
}

void LinkViewLeft(Page *page, View *view, View *left)
{
  PageElement *el = GetPageElement(page, view);
  el->left = GetPageElement(page, left)->view;
}

void LinkViewRight(Page *page, View *view, View *right)
{
  PageElement *el = GetPageElement(page, view);
  el->right = GetPageElement(page, right)->view;
}

void LinkViewBelow(Page *page, View *above, View *below)
{
  LinkViewUp(page, below, above);
  LinkViewDown(page, above, below);
}

void LinkViewBeside(Page *page, View *left, View *right)
{
  LinkViewLeft(page, right, left);
  LinkViewRight(page, left, right);
}


void SelectView(Page *page, View *view)
{
  if (!view) return;
  for (u32 i = 0; i < VecCount(page->elements); i++) {
    if (page->elements[i].view == view) {
      page->selected = i;

      i16 h = view->bounds.bottom - view->bounds.top;
      PlaceCursor(view->bounds.left, view->bounds.top + h/2 + 1);
      ShowCursor();

      return;
    }
  }
}
