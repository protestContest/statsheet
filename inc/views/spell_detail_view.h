#pragma once
#include "spell.h"
#include "view.h"

typedef struct {
  View asView;
  Spell *spell;
  i16 *lineStarts;
  u32 curLine;
} SpellDetailView;

void InitSpellDetailView(SpellDetailView *view, Rect *bounds, Spell *spell);
