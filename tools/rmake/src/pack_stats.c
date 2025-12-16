#include "pack_stats.h"
#include "parse.h"
#include "vec.h"
#include "hashmap.h"
#include "../../inc/stat.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


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


struct {char name; OpCode op;} functionMap[] = {
  {'+', opAdd},
  {'-', opSub},
  {'*', opMul},
  {'/', opDiv},
  {'%', opRem},
  {'&', opAnd},
  {'|', opOr},
  {'~', opNot},
  {'?', opIf},
  {'=', opEq},
  {'<', opLt},
  {'>', opGt},
  {'[', opQuote},
  {']', opReturn},
  {'.', opCall},
  {':', opDup},
  {'_', opDrop},
  {'\\', opSwap},
  {';', opRot},
  {'"', opStr},
  {'!', opStore},
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
} StatInfo;

void FreeStats(StatInfo *stats)
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

void AddDep(u32 id, StatDep **deps)
{
  StatDep *dep = *deps;
  while (dep) {
    if (dep->id == id) {
      return;
    }
    dep = dep->next;
  }
  dep = malloc(sizeof(StatDep));
  dep->id = id;
  dep->next = *deps;
  *deps = dep;
}

char *ParseCalculation(Parser *p, StatDep **deps)
{
  char *code = 0;

  while (!AtEnd(p) && *p->cur != '\n') {
    if (IsDigit(*p->cur)) {
      while (!AtEnd(p) && IsDigit(*p->cur)) {
        VecPush(code, *p->cur);
        p->cur++;
      }
      VecPush(code, opNoop);
      SkipSpaces(p);
    } else if (IsAlpha(*p->cur)) {
      char *start = p->cur;
      while (!AtEnd(p) && IsSymChar(*p->cur)) {
        VecPush(code, *p->cur);
        p->cur++;
      }
      AddDep(Hash(start, p->cur - start), deps);

      VecPush(code, opNoop);
      SkipSpaces(p);
    } else if (*p->cur == opStore) {
      fprintf(stderr, "Store not allowed in stat definitions\n");
      exit(99);
    } else {
      bool found = false;
      for (u32 i = 0; i < ArrayCount(functionMap); i++) {
        if (functionMap[i].name == *p->cur) {
          found = true;
          VecPush(code, functionMap[i].op);
          break;
        }
      }
      if (!found) {
        fprintf(stderr, "Unknown function \"%c\"\n", *p->cur);
        exit(99);
      }
      p->cur++;
      SkipSpaces(p);
    }
  }
  VecPush(code, opHalt);

  return code;
}

StatInfo *ParseStatFile(FILE *f, u32 size)
{
  StatInfo *stats = 0;
  StatInfo stat;

  char *data = malloc(size);
  fread(data, size, 1, f);

  Parser p = {data, data + size, 0};
  SkipWhitespace(&p);

  while (!AtEnd(&p)) {
    if (*p.cur == '#') {
      while (!AtEnd(&p) && *p.cur != '\n') p.cur++;
      if (!AtEnd(&p)) p.cur++;
      continue;
    }
    stat.name = ParseName(&p);
    stat.id = Hash(stat.name, strlen(stat.name));
    SkipSpaces(&p);
    if (AtEnd(&p) || (*p.cur != '=' && *p.cur != ':')) {
      fprintf(stderr, "Expected \"=\" or \":\" in stat definition \"%s\"\n", stat.name);
      exit(99);
    }
    p.cur++;
    SkipSpaces(&p);
    if (AtEnd(&p)) {
      fprintf(stderr, "Expected expression in stat definition \"%s\"\n", stat.name);
      exit(99);
    }
    stat.deps = 0;
    stat.reverseDeps = 0;
    stat.calc = ParseCalculation(&p, &stat.deps);
    if (!stat.calc) {
      fprintf(stderr, "Expected expression in stat definition \"%s\"\n", stat.name);
      exit(99);
    }
    for (u32 i = VecCount(stat.calc); i < Align(VecCount(stat.calc), 4); i++) {
      VecPush(stat.calc, opHalt);
    }

    VecPush(stats, stat);

    SkipWhitespace(&p);
  }

  free(data);
  return stats;
}

void ValidateStats(StatInfo *stats, HashMap *map)
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

void CheckDeps(StatInfo *stat, HashMap *map, HashMap *valid, HashMap *pending, StatInfo *stats, StatInfo ***ordered)
{
  if (HashMapContains(valid, stat->id)) return;
  HashMapSet(pending, stat->id, 1);

  StatDep *dep = stat->deps;
  while (dep) {
    StatInfo *subStat = &stats[HashMapGet(map, dep->id)];

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
  StatInfo *stats = ParseStatFile(f, info->size);
  HashMap map = EmptyHashMap;

  for (u32 i = 0; i < VecCount(stats); i++) {
    HashMapSet(&map, stats[i].id, i);
  }
  ValidateStats(stats, &map);

  HashMap valid = EmptyHashMap;
  HashMap pending = EmptyHashMap;
  StatInfo **ordered = 0;
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
