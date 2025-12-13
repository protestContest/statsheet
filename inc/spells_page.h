#pragma once
#include "page.h"

typedef struct {
  char *name;
  char *school;
  u32 level;
  char *castTime;
  char *duration;
  char *range;
  char *attrs;
  u32 descLen;
  char *description;
} Spell;

Page *InitSpellsPage(void);
