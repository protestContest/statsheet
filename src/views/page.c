#include "views/page.h"
#include "kit/canvas.h"
#include "kit/debug.h"
#include "kit/input.h"
#include "kit/mem.h"
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
  page->selected = 0;
}

Page *BuildPage(u8 **desc)
{
  if (**desc != viewPage) return 0;
  (*desc)++;

  char *title = (char*)*desc;
  while (**desc) (*desc)++;
  (*desc)++;

  Page *page = Alloc(sizeof(Page));
  InitPage(page, title);

  u32 numViews;
  Copy(*desc, &numViews, sizeof(numViews));
  (*desc) += sizeof(numViews);

  Rect links;
  for (u32 i = 0; i < numViews; i++) {
    View *view = BuildView(desc);
    AddPageView(page, view);
    Copy(*desc, &links, sizeof(links));
    (*desc) += sizeof(links);

    if (links.left >= 0) page->elements[VecCount(page->elements)-1].left = links.left;
    if (links.right >= 0) page->elements[VecCount(page->elements)-1].right = links.right;
    if (links.top >= 0) page->elements[VecCount(page->elements)-1].up = links.top;
    if (links.bottom >= 0) page->elements[VecCount(page->elements)-1].down = links.bottom;
  }

  return page;
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

  for (u32 i = 0; i < VecCount(page->elements); i++) {
    View *el = page->elements[i].view;
    DrawView(el);
  }
}

View *GetPageView(Page *page, i32 index)
{
  if (index < 0) return 0;
  return page->elements[index].view;
}

bool PageInput(View *view, u16 input)
{
  Page *page = (Page*)view;
  PageElement *selected = VecAt(page->elements, page->selected);
  if (!selected) return false;

  if (KeyPressed(BTN_LEFT) && selected->left >= 0) {
    SelectView(page, GetPageView(page, selected->left));
  } else if (KeyPressed(BTN_RIGHT) && selected->right >= 0) {
    SelectView(page, GetPageView(page, selected->right));
  } else if (KeyPressed(BTN_UP) && selected->up >= 0) {
    SelectView(page, GetPageView(page, selected->up));
  } else if (KeyPressed(BTN_DOWN) && selected->down >= 0) {
    SelectView(page, GetPageView(page, selected->down));
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
}

void AddPageView(Page *page, View *view)
{
  PageElement el = {view, -1, -1, -1, -1};
  VecPush(page->elements, el);
}

static i32 ViewIndex(Page *page, View *view)
{
  for (u32 i = 0; i < VecCount(page->elements); i++) {
    if (page->elements[i].view == view) {
      return i;
    }
  }
  Error("View not found");
  return -1;
}

void LinkViewUp(Page *page, View *view, View *up)
{
  page->elements[ViewIndex(page, view)].up = ViewIndex(page, up);
}

void LinkViewDown(Page *page, View *view, View *down)
{
  page->elements[ViewIndex(page, view)].down = ViewIndex(page, down);

}
void LinkViewLeft(Page *page, View *view, View *left)
{
  page->elements[ViewIndex(page, view)].left = ViewIndex(page, left);
}

void LinkViewRight(Page *page, View *view, View *right)
{
  page->elements[ViewIndex(page, view)].right = ViewIndex(page, right);

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
