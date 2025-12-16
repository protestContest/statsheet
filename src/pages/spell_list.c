#include "pages/spell_list.h"
#include "kit/canvas.h"
#include "kit/mem.h"
#include "kit/text.h"
#include "views/page.h"
#include "spell.h"
#include "views/spell_row.h"
#include "views/tabs.h"

void InitSpellListPage(Page *page)
{
  i16 lineHeight = LineHeight();
  Rect bounds;
  InitPage(page, "Spellbook");

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
    InitSpellRow(row, &bounds, spell->name);
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
    }
  }
}
