#pragma once
#include "view.h"
#include "spell.h"

typedef struct {
  View asView;
  Spell *spell;
  bool highligted;
} SpellRowView;

void InitSpellRow(SpellRowView *spellRow, Rect *bounds, Spell *spell);

