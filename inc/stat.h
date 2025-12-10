#pragma once
#include "kit.h"

typedef struct {
  u32 id;
  i32 value;
  u32 calcLen;
  u8 *calc;
  u32 numDeps;
  u32 *deps;
} Stat;

void InitStats(void);
Stat *GetStat(char *name);
Stat *GetStatByID(u32 id);

bool UpdateStat(Stat *stat, i32 value);
