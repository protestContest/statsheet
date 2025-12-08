#include "page.h"

void InitPage(Page *page, char *title)
{
  page->title = title;
  page->elements = 0;
  page->focus = 0;
}

void AddPageElement(Page *page, PageElement *element)
{
  element->next = 0;
  if (!page->elements) {
    page->elements = element;
    if (element->onInput) {
      page->focus = element;
      element->active = true;
    }
    return;
  }

  if (!page->focus && element->onInput) {
    page->focus = element;
    element->active = true;
  }

  PageElement *last = page->elements;
  while (last->next) last = last->next;
  last->next = element;
}

void DrawPage(Page *page)
{
  ClearScreen(WHITE);

  FontInfo info;
  GetFontInfo(&info);
  u32 width = TextWidth(page->title);
  Rect rect = {0, 0, SCREEN_W, info.ascent + info.descent};
  FillRect(&rect, DK_GRAY);
  SetColor(WHITE);
  MoveTo(SCREEN_W/2 - width/2, info.ascent);
  Print(page->title);

  PageElement *el = page->elements;
  while (el) {
    el->draw(el);
    el = el->next;
  }
}

void OnPageInput(Page *page, u16 input)
{
  if (KeyPressed(BTN_LEFT)) {
    SelectPrevElement(page);
  } else if (KeyPressed(BTN_RIGHT)) {
    SelectNextElement(page);
  } else if (page->focus) {
    page->focus->onInput(page->focus, input);
  }
}

void SelectNextElement(Page *page)
{
  if (!page->focus) return;

  page->focus->active = false;

  if (!page->focus->next) {
    page->focus = page->elements;
  } else {
    page->focus = page->focus->next;
  }
  if (!page->focus->onInput) {
    SelectNextElement(page);
    return;
  }

  page->focus->active = true;
  page->focus->draw(page->focus);
}

void SelectPrevElement(Page *page)
{
  if (!page->focus) return;

  page->focus->active = false;
  page->focus->draw(page->focus);

  if (page->focus == page->elements) {
    while (page->focus->next) {
      page->focus = page->focus->next;
    }
  } else {
    PageElement *cur = page->elements;
    while (cur->next != page->focus) {
      cur = cur->next;
    }
    page->focus = cur;
  }
  if (!page->focus->onInput) {
    SelectPrevElement(page);
    return;
  }

  page->focus->active = true;
  page->focus->draw(page->focus);
}

void InitPageElement(PageElement *el, Rect *bounds, PageElementDraw draw)
{
  el->bounds = *bounds;
  el->active = false;
  el->draw = draw;
  el->onInput = 0;
  el->next = 0;
}
