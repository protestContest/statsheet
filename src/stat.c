#include "stat.h"
#include "kit/debug.h"
#include "kit/hashmap.h"
#include "kit/math.h"
#include "kit/mem.h"
#include "kit/res.h"
#include "kit/str.h"
#include "kit/vec.h"
#include "prefix.h"

static HashMap statMap = EmptyHashMap;
static Stat *statList = 0;

static void StatUpdated(Stat *stat);

void AddStat(Stat *stat)
{
  VecPush(statList, *stat);
  HashMapSet(&statMap, stat->id, VecCount(statList)-1);
}

static u8 *MatchBracket(u8 *pc)
{
  u32 count = 0;
  while (*pc != opHalt) {
    if (*pc == opReturn) {
      if (count == 0) return pc+1;
      count--;
    } else if (*pc == opQuote) {
      count++;
    }
    pc++;
  }
  Assert(false);
  return 0;
}

void CalcStat(Stat *stat)
{
  if (!stat->calc) return;

  u8 *start;
  u8 *callStack[256] = {0};
  u32 callTop = 0;
  i32 stack[256];
  u32 top = 0;
  u32 index, hash;
  Stat *s;
  i32 n;

  Stat **modified = 0;

  u8 *pc = stat->calc;
  while (*pc != opHalt) {
    if (IsDigit(*pc)) {
      Assert(top < ArrayCount(stack));
      n = *pc - '0';
      pc++;
      while (IsDigit(*pc)) {
        n = n*10 + *pc - '0';
        pc++;
      }
      stack[top++] = n;
      continue;
    }
    if (IsAlpha(*pc)) {
      Assert(top < ArrayCount(stack));
      start = pc;
      while (IsSymChar(*pc)) pc++;
      hash = Hash(start, pc-start);
      Assert(HashMapFetch(&statMap, hash, &index));
      s = &statList[index];
      stack[top++] = s->value;
      continue;
    }
    switch (*pc++) {
    case opNoop:
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
    case opAnd:
      Assert(top >= 2);
      stack[top-2] = stack[top-2] & stack[top-1];
      top--;
      break;
    case opOr:
      Assert(top >= 2);
      stack[top-2] = stack[top-2] | stack[top-1];
      top--;
      break;
    case opNot:
      Assert(top >= 1);
      stack[top-1] = ~stack[top-1];
      break;
    case opIf:
      Assert(top >= 3);
      if (!stack[top-1]) {
        stack[top-3] = stack[top-2];
      }
      top -= 2;
      break;
    case opEq:
      Assert(top >= 2);
      stack[top-2] = (stack[top-2] == stack[top-1]) ? 1 : 0;
      top--;
      break;
    case opLt:
      Assert(top >= 2);
      stack[top-2] = (stack[top-2] < stack[top-1]) ? 1 : 0;
      top--;
      break;
    case opQuote:
      Assert(top < ArrayCount(stack));
      stack[top++] = (i32)pc;
      pc = MatchBracket(pc);
      break;
    case opReturn:
      Assert(callTop > 0);
      pc = callStack[--callTop];
      break;
    case opCall:
      Assert(top >= 1);
      if (*pc != opReturn && *pc != opHalt) {
        Assert(callTop < ArrayCount(callStack));
        callStack[callTop++] = pc;
      }
      pc = (u8*)stack[--top];
      break;
    case opDup:
      Assert(top >= 1);
      Assert(top < ArrayCount(stack));
      stack[top] = stack[top-1];
      top++;
      break;
    case opDrop:
      Assert(top >= 1);
      top--;
      break;
    case opSwap:
      Assert(top >= 2);
      n = stack[top-1];
      stack[top-1] = stack[top-2];
      stack[top-2] = n;
      break;
    case opRot:
      Assert(top >= 3);
      n = stack[top-3];
      stack[top-3] = stack[top-2];
      stack[top-2] = stack[top-1];
      stack[top-1] = n;
      break;
    case opStr:
      Assert(top < ArrayCount(stack));
      start = pc;
      while (*pc != opStr) pc++;
      stack[top++] = Hash(start, pc-start);
      pc++;
      break;
    case opStore:
      Assert(top >= 2);
      Assert(HashMapFetch(&statMap, stack[--top], &index));
      s = &statList[index];
      s->value = stack[--top];
      VecPush(modified, s);
      break;
    default:
      Assert(false);
    }
  }

  for (u32 i = 0; i < VecCount(modified); i++) {
    StatUpdated(modified[i]);
  }
  FreeVec(modified);

  Assert(top == 1);
  stat->value = stack[0];
}

void InitStats(void)
{
  u8 *statData = ResData(GetResource("Stats"));
  u32 numStats = *((u32*)statData);
  statData += sizeof(u32);

  // set vec size to hold all stats, so it won't resize later
  VecMakeRoom(statList, numStats);

  for (u32 i = 0; i < numStats; i++) {
    u32 calcLen;
    Stat stat;
    Copy(statData, &stat.id, sizeof(u32));
    statData += sizeof(u32);
    Copy(statData, &calcLen, sizeof(u32));
    statData += sizeof(u32);
    stat.calc = statData;
    statData += calcLen;
    Copy(statData, &stat.numDeps, sizeof(u32));
    statData += sizeof(u32);
    stat.deps = (u32*)statData;
    statData += sizeof(u32)*stat.numDeps;
    stat.value = 0;

    AddStat(&stat);
  }

  for (u32 i = 0; i < numStats; i++) {
    Stat *stat = &statList[i];
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
    return statList + index;
  }
  return 0;
}

typedef struct StatList {
  Stat *stat;
  struct StatList *next;
} StatList;

static void UpdateStatRec(StatList *queue, StatList *end)
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

static void StatUpdated(Stat *stat)
{
  if (stat->numDeps > 0) {
    StatList *queue = Alloc(sizeof(StatList));
    queue->stat = stat;
    queue->next = 0;
    UpdateStatRec(queue, queue);
  }
}

void UpdateStat(Stat *stat, i32 value)
{
  if (stat->value == value) return;
  stat->value = value;
  StatUpdated(stat);
}
