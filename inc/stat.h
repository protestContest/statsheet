#pragma once

typedef enum {
  opHalt = 0,
  opNoop = ' ',
  opAdd = '+',
  opSub = '-',
  opMul = '*',
  opDiv = '/',
  opRem = '%',
  opAnd = '&',
  opOr = '|',
  opNot = '~',
  opIf = '?',
  opEq = '=',
  opLt = '<',
  opGt = '>',
  opQuote = '[',
  opReturn = ']',
  opCall = '.',
  opDup = ':',
  opDrop = '_',
  opSwap = '\\',
  opRot = ';',
  opStr = '"',
  opStore = '!',
} OpCode;

typedef struct {
  u32 id;
  i32 value;
  u8 *calc;
  u32 numDeps;
  u32 *deps;
} Stat;

void InitStats(void);
Stat *GetStat(char *name);
Stat *GetStatByID(u32 id);

void UpdateStat(Stat *stat, i32 value);
