#include "stat.h"

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
  opSmall = 128,
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
    if (stat->calc[i] & opSmall) {
      Assert(top < ArrayCount(stack));
      n = stat->calc[i] & ~opSmall;
      stack[top++] = n;
      continue;
    }
    switch (stat->calc[i]) {
    case opNoop:
      break;
    case opDefault:
      stat->calc = 0;
      stat->calcLen = 0;
      break;
    case opConst:
      Assert(top < ArrayCount(stack));
      Copy(stat->calc + i + 1, &n, sizeof(u32));
      stack[top++] = n;
      i += 4;
      break;
    case opVar:
      Assert(top < ArrayCount(stack));
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

  // set vec size to hold all stats, so it won't resize later
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

void UpdateStatRec(StatList *queue, StatList *end)
{
  Stat *stat = queue->stat;

  for (u32 i = 0; i < stat->numDeps; i++) {
    u32 depID = stat->deps[i];
    Stat *child = GetStatByID(depID);
    i32 oldValue = child->value;
    CalcStat(child);

    if (child->value != oldValue) {
      StatList *childNode = Alloc(sizeof(StatList));
      childNode->stat = child;
      childNode->next = 0;
      end->next = childNode;
      end = childNode;
    }
  }

  StatList *oldQueue = queue;
  queue = queue->next;
  Free(oldQueue);

  if (queue) {
    UpdateStatRec(queue, end);
  }
}

bool UpdateStat(Stat *stat, i32 value)
{
  if (stat->value == value) return false;
  stat->value = value;
  if (stat->numDeps > 0) {
    StatList *queue = Alloc(sizeof(StatList));

    queue->stat = stat;
    queue->next = 0;
    UpdateStatRec(queue, queue);
    return true;
  }
  return false;
}
