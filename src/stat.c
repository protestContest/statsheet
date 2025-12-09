#include "stat.h"
#include "ui.h"

typedef struct {
  HashMap map;
  Vec items;
} StatMap;

typedef struct StatList {
  Stat *stat;
  struct StatList *next;
} StatList;

static HashMap statMap = EmptyHashMap;
static Vec statList = EmptyVec(Stat);

void AddStat(Stat *stat)
{
  VecPush(&statList, stat);
  HashMapSet(&statMap, stat->id, statList.count-1);
}

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
void CalcStat(Stat *stat)
{
  if (!stat->calc) return;

  i32 stack[256];
  u32 top = 0;
  u32 n, index;
  Stat *s;

  for (u32 i = 0; i < stat->calcLen; i++) {
    switch (stat->calc[i]) {
    case opNoop:
      break;
    case opDefault:
      stat->calc = 0;
      stat->calcLen = 0;
      break;
    case opConst:
      Copy(stat->calc + i + 1, &n, sizeof(u32));
      stack[top++] = n;
      i += 4;
      break;
    case opVar:
      Copy(stat->calc + i + 1, &n, sizeof(u32));
      Assert(HashMapFetch(&statMap, n, &index));
      s = VecAt(&statList, index);
      stack[top++] = s->value;
      i += 4;
      break;
    case opAdd:
      Assert(top >= 2);
      stack[top-2] = stack[top-2] + stack[top-1];
      top--;
      break;
    case opSub:
      Assert(top >= 2);
      stack[top-2] = stack[top-2] - stack[top-1];
      top--;
      break;
    case opMul:
      Assert(top >= 2);
      stack[top-2] = stack[top-2] * stack[top-1];
      top--;
      break;
    case opDiv:
      Assert(top >= 2);
      stack[top-2] = stack[top-2] / stack[top-1];
      top--;
      break;
    case opRem:
      Assert(top >= 2);
      stack[top-2] = stack[top-2] % stack[top-1];
      top--;
      break;
    }
  }

  Assert(top == 1);
  stat->value = stack[0];
}

void InitStats(void)
{
  u8 *statData = ResData(GetResource("Stats"));
  u32 numStats = *((u32*)statData);
  statData += sizeof(u32);

  ResizeVec(&statList, numStats);

  for (u32 i = 0; i < numStats; i++) {
    Stat stat;
    Copy(statData, &stat.id, sizeof(u32));
    statData += sizeof(u32);
    Copy(statData, &stat.calcLen, sizeof(u32));
    statData += sizeof(u32);
    stat.calc = statData;
    statData += stat.calcLen;
    Copy(statData, &stat.numDeps, sizeof(u32));
    statData += sizeof(u32);
    stat.deps = (u32*)statData;
    statData += sizeof(u32)*stat.numDeps;
    stat.value = 0;

    AddStat(&stat);
  }

  for (u32 i = 0; i < numStats; i++) {
    Stat *stat = VecAt(&statList, i);
    CalcStat(stat);
  }
}

Stat *GetStat(char *name)
{
  return GetStatByID(Hash(name, StrLen(name)));
}

Stat *GetStatByID(u32 id)
{
  u32 index;
  if (HashMapFetch(&statMap, id, &index)) {
    return (Stat*)VecAt(&statList, index);
  }
  return 0;
}

void StatListAppend(StatList *list, Stat *stat)
{
  while (list->next) list = list->next;
  list->next = Alloc(sizeof(StatList));
  list->next->stat = stat;
  list->next->next = 0;
}

void UpdateStatRec(StatList *queue)
{
  Stat *stat = queue->stat;
  for (u32 i = 0; i < stat->numDeps; i++) {
    u32 dep = stat->deps[i];
    Stat *child = GetStatByID(dep);
    CalcStat(child);
    StatListAppend(queue, child);
  }

  StatList *oldQueue = queue;
  queue = queue->next;
  Free(oldQueue);
  if (queue) {
    UpdateStatRec(queue);
  }
}

void UpdateStat(Stat *stat, i32 value)
{
  stat->value = value;
  if (stat->numDeps > 0) {
    StatList *queue = Alloc(sizeof(StatList));
    queue->stat = stat;
    queue->next = 0;
    UpdateStatRec(queue);
  }
}
