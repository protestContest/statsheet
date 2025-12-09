#include "pack_stats.h"
#include "parse.h"
#include "vec.h"
#include "hashmap.h"
#include <string.h>

/*
A stat file has this format:

u32 numStats;
For each stat:
  u32 id;
  u32 codeLen;
  u8 code[codeLen];
  u32 numDeps;
  u32 depIDs[numDeps];
*/

typedef enum {
  opNoop = ' ',
  opDefault = ':',
  opConst = '#',
  opVar = '$',
  opAdd = '+',
  opSub = '-',
  opMul = '*',
  opDiv = '/',
  opRem = '%',
} OpCode;

struct {char *name; OpCode op;} functionMap[] = {
  {"+", opAdd},
  {"-", opSub},
  {"*", opMul},
  {"/", opDiv},
  {"%", opRem},
};

typedef struct StatDep {
  u32 id;
  struct StatDep *next;
} StatDep;

typedef struct {
  u32 id;
  char *name;
  char *calc;
  StatDep *deps;
  u32 numDeps;
  StatDep *reverseDeps;
} Stat;

void FreeStats(Stat *stats)
{
  for (u32 i = 0; i < VecCount(stats); i++) {
    free(stats[i].name);
    FreeVec(stats[i].calc);
    StatDep *dep = stats[i].deps;
    while (dep) {
      StatDep *next = dep->next;
      free(dep);
      dep = next;
    }
  }
  FreeVec(stats);
}

char *ParseCalculation(char **cur, char *end, StatDep **deps)
{
  char *code = 0;

  while (*cur < end && **cur != '\n') {
    if (IsDigit(**cur)) {
      u32 num = 0;
      while (*cur < end && IsDigit(**cur)) {
        num = num*10 + **cur - '0';
        (*cur)++;
      }
      VecPush(code, opConst);
      VecPush(code, num & 0xFF);
      VecPush(code, (num >> 8) & 0xFF);
      VecPush(code, (num >> 16) & 0xFF);
      VecPush(code, (num >> 24) & 0xFF);
      SkipSpaces(*cur, end);
    } else if (IsUppercase(**cur)) {
      char *start = *cur;
      while (*cur < end && IsSymChar(**cur)) {
        (*cur)++;
      }
      StatDep *dep = malloc(sizeof(StatDep));
      dep->id = Hash(start, *cur - start);
      dep->next = *deps;
      *deps = dep;

      VecPush(code, opVar);
      VecPush(code, dep->id & 0xFF);
      VecPush(code, (dep->id >> 8) & 0xFF);
      VecPush(code, (dep->id >> 16) & 0xFF);
      VecPush(code, (dep->id >> 24) & 0xFF);
      SkipSpaces(*cur, end);
    } else {
      char *start = *cur;
      while (*cur < end && !IsWhitespace(**cur)) {
        (*cur)++;
      }
      char *fn = malloc(*cur - start + 1);
      memcpy(fn, start, *cur-start);
      fn[*cur-start] = 0;

      bool found = false;
      for (u32 i = 0; i < ArrayCount(functionMap); i++) {
        if (strcmp(functionMap[i].name, fn) == 0) {
          found = true;
          VecPush(code, functionMap[i].op);
          break;
        }
      }
      free(fn);
      if (!found) {
        fprintf(stderr, "Unknown function \"%s\"\n", fn);
        exit(99);
      }
      SkipSpaces(*cur, end);
    }
  }

  return code;
}

Stat *ParseStatFile(FILE *f, u32 size)
{
  Stat *stats = 0;
  Stat stat;

  char *data = malloc(size);
  fread(data, size, 1, f);

  char *end = data + size;
  char *cur = SkipWhitespace(data, end);

  while (cur < end) {
    bool isDefault = false;
    stat.name = ParseName(&cur, end);
    stat.id = Hash(stat.name, strlen(stat.name));
    SkipSpaces(cur, end);
    if (cur == end || (*cur != '=' && *cur != ':')) {
      fprintf(stderr, "Expected \"=\" or \":\" in stat definition \"%s\"\n", stat.name);
      exit(99);
    }
    if (*cur == ':') {
      isDefault = true;
    }
    cur++;
    SkipSpaces(cur, end);
    if (cur == end) {
      fprintf(stderr, "Expected expression in stat definition \"%s\"\n", stat.name);
      exit(99);
    }
    stat.deps = 0;
    stat.reverseDeps = 0;
    stat.calc = ParseCalculation(&cur, end, &stat.deps);
    if (!stat.calc) {
      fprintf(stderr, "Expected expression in stat definition \"%s\"\n", stat.name);
      exit(99);
    }
    if (isDefault) {
      VecPush(stat.calc, opDefault);
    }
    for (u32 i = VecCount(stat.calc); i < Align(VecCount(stat.calc), 4); i++) {
      VecPush(stat.calc, opNoop);
    }

    VecPush(stats, stat);

    cur = SkipWhitespace(cur, end);
  }

  free(data);
  return stats;
}

void ValidateStats(Stat *stats, HashMap *map)
{
  for (u32 i = 0; i < VecCount(stats); i++) {
    StatDep *deps = stats[i].deps;
    u32 depNum = 1;
    while (deps) {
      if (!HashMapContains(map, deps->id)) {
        fprintf(stderr, "Missing dependency #%d for \"%s\"\n", depNum, stats[i].name);
        exit(99);
      }
      depNum++;
      deps = deps->next;
    }
  }
}

void CheckDeps(Stat *stat, HashMap *map, HashMap *valid, HashMap *pending, Stat *stats, Stat ***ordered)
{
  if (HashMapContains(valid, stat->id)) return;
  HashMapSet(pending, stat->id, 1);

  StatDep *dep = stat->deps;
  while (dep) {
    Stat *subStat = &stats[HashMapGet(map, dep->id)];
    if (HashMapContains(pending, dep->id)) {
      fprintf(stderr, "Circular dependency found between \"%s\" and \"%s\"\n", stat->name, subStat->name);
      exit(99);
    } else if (!HashMapContains(valid, dep->id)) {
      CheckDeps(subStat, map, valid, pending, stats, ordered);
    }
    StatDep *revDep = malloc(sizeof(StatDep));
    revDep->id = stat->id;
    revDep->next = subStat->reverseDeps;
    subStat->reverseDeps = revDep;

    dep = dep->next;
  }

  HashMapDelete(pending, stat->id);
  HashMapSet(valid, stat->id, 1);
  VecPush(*ordered, stat);
}

void PackStats(ResInfo *info, FILE *f)
{
  Stat *stats = ParseStatFile(f, info->size);
  HashMap map = EmptyHashMap;

  for (u32 i = 0; i < VecCount(stats); i++) {
    HashMapSet(&map, stats[i].id, i);
  }
  ValidateStats(stats, &map);

  HashMap valid = EmptyHashMap;
  HashMap pending = EmptyHashMap;
  Stat **ordered = 0;
  for (u32 i = 0; i < VecCount(stats); i++) {
    CheckDeps(&stats[i], &map, &valid, &pending, stats, &ordered);
  }

  info->size = sizeof(u32); // header
  info->size += sizeof(u32); // numStats
  for (u32 i = 0; i < VecCount(stats); i++) {
    StatDep *dep = stats[i].reverseDeps;
    stats[i].numDeps = 0;
    while (dep) {
      stats[i].numDeps++;
      dep = dep->next;
    }

    info->size += sizeof(u32); // id
    info->size += sizeof(u32); // codeLen
    info->size += VecCount(stats[i].calc); // code
    info->size += sizeof(u32); // numDeps
    info->size += sizeof(u32)*stats[i].numDeps; // deps
  }

  info->data = malloc(info->size);
  *((u32*)info->data) = (info->size - sizeof(u32)) << 8;
  u8 *cur = info->data + sizeof(u32);

  u32 numStats = VecCount(stats);
  memcpy(cur, &numStats, sizeof(numStats));
  cur += sizeof(numStats);

  for (u32 i = 0; i < numStats; i++) {
    memcpy(cur, &ordered[i]->id, sizeof(ordered[i]->id));
    cur += sizeof(ordered[i]->id);
    u32 codeLen = VecCount(ordered[i]->calc);
    memcpy(cur, &codeLen, sizeof(codeLen));
    cur += sizeof(codeLen);
    memcpy(cur, ordered[i]->calc, codeLen);
    cur += codeLen;
    memcpy(cur, &ordered[i]->numDeps, sizeof(ordered[i]->numDeps));
    cur += sizeof(ordered[i]->numDeps);
    StatDep *dep = ordered[i]->reverseDeps;
    for (u32 j = 0; j < ordered[i]->numDeps; j++) {
      memcpy(cur, &dep->id, sizeof(dep->id));
      cur += sizeof(dep->id);
      dep = dep->next;
    }
  }

  DestroyHashMap(&map);
  DestroyHashMap(&valid);
  DestroyHashMap(&pending);
  FreeStats(stats);
}
