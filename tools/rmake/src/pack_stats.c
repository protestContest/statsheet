#include "pack_stats.h"
#include "parse.h"
#include "vec.h"
#include "hashmap.h"
#include "util.h"
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


static bool ValidOp(u8 op)
{
  switch (op) {
  case opAdd:
  case opSub:
  case opMul:
  case opDiv:
  case opRem:
  case opAnd:
  case opOr:
  case opNot:
  case opIf:
  case opEq:
  case opLt:
  case opGt:
  case opQuote:
  case opReturn:
  case opCall:
  case opDup:
  case opDrop:
  case opSwap:
  case opRot:
  case opStr:
  case opStore:
    return true;
  default:
    return false;
  }
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

  while (!AtEnd(p) && Peek(p) != '\n') {
    if (IsDigit(Peek(p))) {
      while (!AtEnd(p) && IsDigit(Peek(p))) {
        VecPush(code, Peek(p));
        Adv(p);
      }
      VecPush(code, opNoop);
      SkipSpaces(p);
    } else if (IsAlpha(Peek(p))) {
      char *start = p->cur;
      while (!AtEnd(p) && IsSymChar(Peek(p))) {
        VecPush(code, Peek(p));
        Adv(p);
      }
      AddDep(Hash(start, p->cur - start), deps);

      VecPush(code, opNoop);
      SkipSpaces(p);
    } else if (Peek(p) == opStore) {
      fprintf(stderr, "Store not allowed in stat definitions\n");
      exit(99);
    } else {
      if (!ValidOp(Peek(p))) {
        fprintf(stderr, "Unknown op \"%c\" at %s:%d:%d\n", Peek(p), p->file, p->line+1, p->col+1);
        exit(99);
      }
      VecPush(code, Peek(p));
      Adv(p);
      SkipSpaces(p);
    }
  }
  VecPush(code, opHalt);

  return code;
}

StatInfo *ParseStatFile(char *path, u32 size)
{
  FILE *f = fopen(path, "rb");
  StatInfo *stats = 0;
  StatInfo stat;

  char *data = malloc(size);
  fread(data, size, 1, f);
  fclose(f);

  Parser p = {data, data + size, path, 0, 0};
  SkipWhitespace(&p);

  while (!AtEnd(&p)) {
    if (Peek(&p) == '#') {
      SkipLine(&p);
      SkipWhitespace(&p);
      continue;
    }
    stat.name = ParseName(&p);
    stat.id = Hash(stat.name, strlen(stat.name));
    SkipSpaces(&p);
    Expect(&p, "=");

    SkipSpaces(&p);
    Expect(&p, "");

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

void PackStats(ResInfo *info)
{
  StatInfo *stats = ParseStatFile(info->path, info->size);
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
