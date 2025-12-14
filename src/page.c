// #include "page.h"
// #include "ui.h"

// void InitPage(Page *page, char *title)
// {
//   Rect bounds = {0, 0, SCREEN_W, SCREEN_H};
//   InitView(&page->asView, &bounds, DrawPage, PageInput, ActivatePage);
//   page->title = title;
//   InitVec(&page->elements, sizeof(PageElement*), 0);
//   page->selected = 0;
// }

// void AddPageElement(Page *page, PageElement *element)
// {
//   VecPush(&page->elements, &element);
// }

// PageElement *GetPageElement(Page *page, u32 index)
// {
//   PageElement **el = VecAt(&page->elements, index);
//   return *el;
// }

// void SelectElement(Page *page, PageElement *element)
// {
//   page->selected = element;
//   PlaceCursor(element->asView.bounds.left, element->asView.bounds.top + 7);
//   ShowCursor();
// }

// void LinkPage(Page *left, Page *right)
// {
//   if (left->next && left->next->prev == left) {
//     left->next->prev = 0;
//   }
//   if (right->prev && right->prev->next == right) {
//     right->prev->next = 0;
//   }
//   left->next = right;
//   right->prev = left;
// }

// void DrawPage(View *view)
// {
//   Page *page = (Page*)view;
//   FontInfo info;
//   GetFontInfo(&info);
//   u32 width = TextWidth(page->title);
//   Rect rect = {0, 0, SCREEN_W, info.ascent + info.descent};
//   FillRect(&rect, DK_GRAY);
//   SetColor(WHITE);
//   MoveTo(SCREEN_W/2 - width/2, info.ascent);
//   Print(page->title);

//   for (u32 i = 0; i < page->elements.count; i++) {
//     View **el = VecAt(&page->elements, i);
//     (*el)->draw(*el);
//   }
// }

// void PageInput(View *view, u16 input)
// {
//   Page *page = (Page*)view;
//   if (!page->selected) return;

//   if (KeyPressed(BTN_LEFT) && page->selected->left) {
//     SelectElement(page, page->selected->left);
//   } else if (KeyPressed(BTN_RIGHT) && page->selected->right) {
//     SelectElement(page, page->selected->right);
//   } else if (KeyPressed(BTN_UP) && page->selected->up) {
//     SelectElement(page, page->selected->up);
//   } else if (KeyPressed(BTN_DOWN) && page->selected->down) {
//     SelectElement(page, page->selected->down);
//   } else {
//     page->selected->asView.onInput(&page->selected->asView, input);
//   }
// }

// void ActivatePage(View *view, bool active)
// {
//   Page *page = (Page*)view;

//   if (active) {
//     ClearScreen();
//     DrawPage(view);
//   } else {
//     HideAllObjs();
//   }

//   if (page->selected) {
//     if (page->selected->asView.activate) {
//       page->selected->asView.activate(&page->selected->asView, active);
//     }
//   }
// }

// static Page *curPage = 0;

// void SelectPage(Page *page)
// {
//   if (curPage) {
//     curPage->asView.activate(&curPage->asView, false);
//   }
//   curPage = page;
//   curPage->asView.activate(&curPage->asView, true);
//   RedrawPage();
// }

// void NextPage(void)
// {
//   if (!curPage->next) return;
//   SelectPage(curPage->next);
// }

// void PrevPage(void)
// {
//   if (!curPage->prev) return;
//   SelectPage(curPage->prev);
// }

// void RedrawPage(void)
// {
//   Assert(curPage);
//   curPage->asView.draw(&curPage->asView);
// }

// void ReactivatePage(void)
// {
//   curPage->asView.activate(&curPage->asView, true);
// }

// void HandleInput(u16 input)
// {
//   curPage->asView.onInput(&curPage->asView, input);
// }
