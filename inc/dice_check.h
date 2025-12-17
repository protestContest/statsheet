#pragma once
#include "stat.h"

enum {d4, d6, d8, d10, d12, d20};

void DiceCheck(char *title, u32 die, i32 mod);
void StatDiceCheck(char *title, Stat *stat);
