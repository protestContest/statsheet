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

void AddStat(Stat *stat)
{
  VecPush(statList, *stat);
  HashMapSet(&statMap, stat->id, VecCount(statList)-1);
}

typedef enum {
  opHalt = 0,
  opNoop = ' ',
  opConst = '#',
  opSmall = 128,
  opVar = '$',
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
  opBlockStart = '[',
  opBlockEnd = ']',
  opCall = '.',
  opDup = '"',
  opDrop = '_',
  opSwap = '\\',
  opRot = '>',
} OpCode;

static u8 *MatchBracket(u8 *pc)
{
  u32 count = 0;
  while (*pc != opHalt) {
    if (*pc == opBlockEnd) {
      if (count == 0) return pc+1;
      count--;
    } else if (*pc == opBlockStart) {
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

  u8 *callStack[256] = {0};
  u32 callTop = 0;
  i32 stack[256];
  u32 top = 0;

  i32 n;
  u32 index;
  Stat *s;

  u8 *pc = stat->calc;
  while (*pc != opHalt) {
    if (*pc & opSmall) {
      Assert(top < ArrayCount(stack));
      n = (*pc) & ~opSmall;
      stack[top++] = n;
      pc++;
      continue;
    }
    switch (*pc++) {
    case opConst:
      Assert(top < ArrayCount(stack));
      Copy(pc, &n, sizeof(u32));
      stack[top++] = n;
      pc += sizeof(u32);
      break;
    case opVar:
      Assert(top < ArrayCount(stack));
      Copy(pc, &n, sizeof(u32));
      Assert(HashMapFetch(&statMap, n, &index));
      s = &statList[index];
      stack[top++] = s->value;
      pc += sizeof(u32);
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
    case opBlockStart:
      Assert(top < ArrayCount(stack));
      stack[top++] = (u32)pc;
      pc = MatchBracket(pc);
      break;
    case opBlockEnd:
      Assert(callTop > 0);
      pc = callStack[--callTop];
      break;
    case opCall:
      Assert(top >= 1);
      if (*pc != opBlockEnd && *pc != opHalt) {
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
    default:
      Assert(false);
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
