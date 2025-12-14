// #include "spells_page.h"
// #include "elements.h"
// #include "kit/str.h"
// #include "kit/text.h"
// #include "page.h"
// #include "ui.h"

// static Page spellsPage, spellPage;
// static Vec spells = EmptyVec(Spell);

// static void InitSpells(void)
// {
//   u8 *res = GetResource("Spells");
//   char *cur = ResData(res);
//   char *end = cur + ResLength(res);

//   ResizeVec(&spells, 8);

//   Spell spell;
//   while (cur < end) {
//     spell.name = cur;
//     while (cur < end && *cur != 0) cur++;
//     if (cur == end) break;
//     cur++;
//     spell.school = cur;
//     while (cur < end && *cur != 0) cur++;
//     if (cur == end) break;
//     cur++;
//     spell.level = ParseInt(&cur);
//     if (cur == end || *cur != 0) break;
//     cur++;
//     spell.castTime = cur;
//     while (cur < end && *cur != 0) cur++;
//     if (cur == end) break;
//     cur++;
//     spell.duration = cur;
//     while (cur < end && *cur != 0) cur++;
//     if (cur == end) break;
//     cur++;
//     spell.range = cur;
//     while (cur < end && *cur != 0) cur++;
//     if (cur == end) break;
//     cur++;
//     spell.attrs = cur;
//     while (cur < end && *cur != 0) cur++;
//     if (cur == end) break;
//     cur++;
//     spell.description = cur;
//     while (cur < end && *cur != 0) cur++;
//     cur++;
//     VecPush(&spells, &spell);

//     while (cur < end && (*cur == ' ' || *cur == '\n')) cur++;
//     if (cur == end) break;
//   }
// }

// static void ShowSpellPage(Spell *spell)
// {
//   InitPage(&spellPage, spell->name);

//   Rect bounds;
//   SetRect(&bounds, 0, 15, SCREEN_W, SCREEN_H);

//   PageElement *text = NewTextElement(&bounds, spell->description);
//   AddPageElement(&spellPage, text);

//   HideAllObjs();
//   ClearScreen();
//   spellPage.asView.draw(&spellPage.asView);
//   text->asView.draw(&text->asView);

//   while (true) {
//     VSync();
//     u16 input = GetInput();
//     if (KeyPressed(BTN_B)) break;
//     text->asView.onInput(&text->asView, input);
//   }

//   VecPop(&spellPage.elements, 0);
//   Free(text);
//   RedrawPage();
//   ReactivatePage();
// }

// typedef struct {
//   PageElement asElement;
//   u32 spell;
// } SpellRowElement;

// static void DrawSpellRow(View *view)
// {
//   FontInfo info;
//   GetFontInfo(&info);
//   SpellRowElement *row = (SpellRowElement*)view;
//   Spell *spell = VecAt(&spells, row->spell);
//   Assert(spell);
//   SetColor(BLACK);
//   MoveTo(view->bounds.left, view->bounds.top + info.ascent);
//   Print(spell->name);
//   MoveTo(view->bounds.left+120, view->bounds.top + info.ascent);
//   if (spell->level == 0) {
//     Print("C");
//   } else {
//     char levelStr[4] = {0};
//     CopyStr("L^", levelStr);
//     FormatInt(levelStr, spell->level, 4);
//     Print(levelStr);
//   }
//   MoveTo(view->bounds.left+150, view->bounds.top + info.ascent);
//   Print(spell->castTime);
//   MoveTo(view->bounds.left+200, view->bounds.top + info.ascent);
//   Print(spell->attrs);
// }

// static void SpellRowInput(View *view, u16 input)
// {
//   Page *page = (Page*)view;
//   if (KeyPressed(BTN_A)) {
//     SpellRowElement *row = (SpellRowElement*)page->selected;
//     ShowSpellPage(VecAt(&spells, row->spell));
//   }
// }

// PageElement *NewSpellRowElement(Rect *bounds, u32 spellNum)
// {
//   SpellRowElement *row = Alloc(sizeof(SpellRowElement));
//   InitPageElement((PageElement*)row, bounds, DrawSpellRow, SpellRowInput);
//   row->spell = spellNum;
//   return (PageElement*)row;
// }

// static void SpellsPageActivate(View *view, bool active)
// {
//   Page *page = (Page*)view;
//   ActivatePage(view, active);
//   if (active) {
//     SelectElement(page, page->selected);
//   }
// }

// Page *InitSpellsPage(void)
// {
//   Rect bounds;
//   FontInfo info;
//   GetFontInfo(&info);
//   u32 lineHeight = info.ascent + info.descent;

//   InitSpells();

//   Page *page = &spellsPage;
//   InitPage(page, "Spells");
//   page->asView.activate = SpellsPageActivate;

//   SetRect(&bounds, 8, 15, SCREEN_W/2-4, 15 + lineHeight);

//   PageElement *lastEl = 0;
//   for (u32 i = 0; i < spells.count; i++) {
//     PageElement *el = NewSpellRowElement(&bounds, i);
//     AddPageElement(page, el);
//     if (i == 0) {
//       SelectElement(page, el);
//     } else {
//       LinkElementBelow(lastEl, el);
//     }
//     lastEl = el;
//     OffsetRect(&bounds, 0, lineHeight);
//   }

//   return page;
// }
