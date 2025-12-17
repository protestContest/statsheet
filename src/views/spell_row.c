#include "views/spell_row.h"
#include "kit/str.h"
#include "kit/text.h"

void SpellRowDraw(View *view)
{
  FontInfo info;
  GetFontInfo(&info);
  SpellRowView *row = (SpellRowView*)view;

  if (row->highligted) {
    FillRect(&view->bounds, GRAY);
  }

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

  i16 cx = (view->bounds.left + view->bounds.right)/2;
  MoveTo(cx, view->bounds.top + info.ascent);
  if (row->spell->level == 0) {
    Print("C");
  } else {
    char n[8] = {0};
    NumToString(row->spell->level, n);
    Print(n);
  }

  MoveTo(cx + 20, view->bounds.top + info.ascent);
  Print(row->spell->castTime);
  MoveTo(cx + 60, view->bounds.top + info.ascent);
  Print(row->spell->range);
  if (row->spell->ritual) {
    MoveTo(view->bounds.right - TextWidth("RC"), view->bounds.top + info.ascent);
    Print("R");
  }
  if (row->spell->concentration) {
    MoveTo(view->bounds.right - TextWidth("C"), view->bounds.top + info.ascent);
    Print("C");
  }
}

void InitSpellRow(SpellRowView *spellRow, Rect *bounds, Spell *spell)
{
  InitView(&spellRow->asView, bounds, SpellRowDraw, 0, 0, 0);
  spellRow->spell = spell;
  spellRow->highligted = false;
}
