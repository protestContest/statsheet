#pragma once
#include "spell.h"
#include "view.h"
#include "views/spell_row.h"
#include "views/text_view.h"
#include "views/label.h"

typedef struct {
  View asView;
  Spell *spell;
  SpellRowView title;
  TextView content;
  Label castBtn;
  Label prepBtn;
  View *selected;
} SpellDetailView;

void InitSpellDetailView(SpellDetailView *view, Rect *bounds, Spell *spell);
