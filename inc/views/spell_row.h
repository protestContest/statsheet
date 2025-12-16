#pragma once
#include "view.h"
#include "spell.h"

typedef struct {
  View asView;
  Spell *spell;
} SpellRowView;

void InitSpellRow(SpellRowView *spellRow, Rect *bounds, char *spellName);

