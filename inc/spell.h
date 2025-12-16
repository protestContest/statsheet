#pragma once

typedef enum {
  abjuration,
  conjuration,
  divination,
  enchantment,
  evocation,
  illusion,
  necromancy,
  transmutation
} SpellSchool;

typedef struct {
  char *name;
  SpellSchool school;
  u16 level;
  char *castTime;
  char *duration;
  char *range;
  bool ritual;
  bool concentration;
  char *description;
} Spell;

void InitSpells(void);
Spell *GetSpell(char *name);
Spell *GetIndSpell(u32 i);
u32 NumSpells(void);
void PrepareSpell(char *name);
void UnprepareSpell(char *name);
bool IsSpellPrepared(char *name);
