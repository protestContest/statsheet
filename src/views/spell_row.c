#include "views/spell_row.h"
#include "kit/str.h"
#include "kit/text.h"

void SpellRowDraw(View *view)
{
  FontInfo info;
  GetFontInfo(&info);
  SpellRowView *row = (SpellRowView*)view;
  if (row->spell->school == divination) {
    SetColor(RED);
  } else {
    SetColor(BLACK);
  }
  if (IsSpellPrepared(row->spell->name)) {
    MoveTo(view->bounds.left, view->bounds.top + info.ascent);
    Print("*");
  } else {
    MoveTo(view->bounds.left + TextWidth("*"), view->bounds.top + info.ascent);
  }
  Print(row->spell->name);
  MoveTo(view->bounds.left + 120, view->bounds.top + info.ascent);
  if (row->spell->level == 0) {
    Print("C");
  } else {
    char n[8] = {0};
    NumToString(row->spell->level, n);
    Print(n);
  }
}

void InitSpellRow(SpellRowView *spellRow, Rect *bounds, char *spellName)
{
  InitView(&spellRow->asView, bounds, SpellRowDraw, 0, 0);
  spellRow->spell = GetSpell(spellName);
}
