#pragma once
#include "kit.h"

typedef enum {simpleStat, modStat, abilityStat, hpStat, chargeStat} StatType;

struct Stat;
struct StatMap;
typedef u32 (*StatCalc)(struct Stat *stat, struct StatMap *stats);

typedef struct Stat {
  StatType type;
  char *name;
  i32 value;
  i32 tempMod;
  i32 max;
  char *calculate;
} Stat;

void AddStat(Stat *stat, char *name, StatType type, u32 value);
Stat *GetStat(char *name);
void PrintStat(Stat *stat, Rect *bounds);
void EditStat(Stat *stat, Rect *bounds);
