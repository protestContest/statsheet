#include "pages/spell_list.h"
#include "kit/canvas.h"
#include "kit/input.h"
#include "kit/mem.h"
#include "kit/text.h"
#include "views/page.h"
#include "spell.h"
#include "views/spell_detail_view.h"
#include "views/spell_row.h"
#include "views/tab_view.h"

static void ShowSpell(Spell *spell)
{
  SpellDetailView detail;
  i16 w = 230;
  i16 h = 150;
  Rect bounds = {SCREEN_W/2 - w/2, SCREEN_H/2 - h/2, SCREEN_W/2 + w/2, SCREEN_H/2+h/2};
  InitSpellDetailView(&detail, &bounds, spell);
  Run(&detail.asView);
  DestroyView(&detail.asView);
}

static bool PageViewInput(View *view, u16 input)
{
  Page *page = (Page*)view;
  TabView *tabView = (TabView*)page->views[0];
  PageInput(view, input);
  if (KeyPressed(BTN_LEFT)) {
    SelectTab(tabView, tabView->selectedTab-1);
    SelectView(page, GetTabView(tabView, 0));
  } else if (KeyPressed(BTN_RIGHT)) {
    SelectTab(tabView, tabView->selectedTab+1);
    SelectView(page, GetTabView(tabView, 0));
  } else if (KeyPressed(BTN_A)) {
    SpellRowView *row = (SpellRowView*)page->elements[page->selected].view;
    ShowSpell(row->spell);
  }
  return false;
}

void InitSpellListPage(Page *page)
{
  i16 lineHeight = LineHeight();
  Rect bounds;
  InitPage(page, "Spellbook");
  page->asView.onInput = PageViewInput;

  bounds = page->asView.bounds;
  bounds.top += 15;
  TabView *tabView = Alloc(sizeof(TabView));
  InitTabView(tabView, &bounds);
  AddPageView(page, &tabView->asView);
  u32 tabNum = 0;

  SetRect(&bounds, 8, 15, SCREEN_W-8, 15+lineHeight);

  View *lastView = 0;
  for (u32 i = 0; i < NumSpells(); i++) {
    Spell *spell = GetIndSpell(i);
    SpellRowView *row = Alloc(sizeof(SpellRowView));
    InitSpellRow(row, &bounds, spell);
    AddTabView(tabView, tabNum, &row->asView);

    if (lastView) {
      LinkViewBelow(page, lastView, &row->asView);
    } else {
      SelectView(page, &row->asView);
    }
    lastView = &row->asView;
    OffsetRect(&bounds, 0, lineHeight);

    if (bounds.bottom > SCREEN_H - 8) {
      tabNum++;
      SetRect(&bounds, 8, 15, SCREEN_W-8, 15+lineHeight);
      lastView = 0;
    }
  }
}
