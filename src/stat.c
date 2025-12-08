#include "stat.h"
#include "ui.h"

static void PrintSimpleStat(Stat *stat, Rect *bounds);
static void EditSimpleStat(Stat *stat, Rect *bounds);
static void PrintAbilityStat(Stat *stat, Rect *bounds);
static void PrintChargeStat(Stat *stat, Rect *bounds);
static void EditCharges(Stat *stat, Rect *bounds);

typedef struct StatMap {
  HashMap map;
  u32 count;
  u32 capacity;
  Stat **items;
} StatMap;

static StatMap statMap = {EmptyHashMap, 0, 0, 0};

static void RegisterStat(Stat *stat)
{
  if (statMap.count + 1 > statMap.capacity) {
    statMap.capacity = statMap.capacity ? 2*statMap.capacity : 2;
    statMap.items = Realloc(statMap.items, sizeof(Stat*)*statMap.capacity);
  }

  u32 index = statMap.count++;
  statMap.items[index] = stat;

  u32 key = Hash(stat->name, StrLen(stat->name));
  HashMapSet(&statMap.map, key, index);
}

Stat *GetStat(char *name)
{
  u32 index;
  if (HashMapFetch(&statMap.map, Hash(name, StrLen(name)), &index)) {
    return statMap.items[index];
  }
  return 0;
}

void AddStat(Stat *stat, char *name, StatType type, u32 value)
{
  stat->type = type;
  stat->name = name;
  stat->value = value;
  stat->calculate = 0;

  RegisterStat(stat);
}

void PrintStat(Stat *stat, Rect *bounds)
{
  switch (stat->type) {
  case abilityStat:
    PrintAbilityStat(stat, bounds);
    break;
  case chargeStat:
    PrintChargeStat(stat, bounds);
    break;
  default:
    PrintSimpleStat(stat, bounds);
  }
}

void EditStat(Stat *stat, Rect *bounds)
{
  u32 w;

  switch (stat->type) {
  case abilityStat:
    w = TextWidth("00");
    bounds->right -= w + 8;
    bounds->left = bounds->right - w;
    EditSimpleStat(stat, bounds);
    break;
  case chargeStat:
    bounds->left = bounds->right - 9*stat->max - 1;
    bounds->bottom = bounds->top + 10;
    EditCharges(stat, bounds);
    break;
  default:
    bounds->left = bounds->right - TextWidth("00");
    EditSimpleStat(stat, bounds);
  }
}

static void DrawStatValue(i32 value, Rect *bounds)
{
  char numStr[8] = {0};
  FontInfo info;
  GetFontInfo(&info);
  NumToString(value, numStr);
  MoveTo(bounds->right - TextWidth(numStr), bounds->top + info.ascent);
  Print(numStr);
}

static void EditSimpleStat(Stat *stat, Rect *bounds)
{
  i32 originalValue = stat->value;
  FontInfo info;
  GetFontInfo(&info);
  SetColor(BLACK);
  char numStr[8] = {0};
  Rect contentRect = *bounds;
  InsetRect(&contentRect, -2, -2);
  FrameRect(&contentRect);
  InsetRect(&contentRect, 1, 1);

  PlaceObj(arrowLeftObj, contentRect.left - 7, contentRect.top + 3);
  SetObjDisplay(arrowLeftObj, ShowObj);
  PlaceObj(arrowRightObj, contentRect.right - 1, contentRect.top + 3);
  SetObjDisplay(arrowRightObj, ShowObj);

  while (true) {
    VSync();
    u16 input = GetInput();

    if (KeyPressed(BTN_A)) {
      break;
    } else if (KeyPressed(BTN_B) || KeyPressed(BTN_SELECT)) {
      stat->value = originalValue;
      break;
    } else if (KeyPressed(BTN_LEFT)) {
      stat->value--;
    } else if (KeyPressed(BTN_RIGHT)) {
      stat->value++;
    }

    if (input & (BTN_LEFT | BTN_RIGHT)) {
      FillRect(&contentRect, WHITE);
      DrawStatValue(stat->value, &contentRect);
    }
  }

  FillRect(&contentRect, WHITE);
  ClearStr(numStr);
  NumToString(stat->value, numStr);
  MoveTo(bounds->right - TextWidth(numStr), bounds->top + info.ascent);
  Print(numStr);

  SetObjDisplay(arrowLeftObj, HideObj);
  SetObjDisplay(arrowRightObj, HideObj);

  InsetRect(&contentRect, -1, -1);
  SetColor(WHITE);
  FrameRect(&contentRect);
}

static void DrawCharges(i32 value, i32 max, Rect *bounds)
{
  FrameRect(bounds);
  for (i32 i = 1; i < max; i++) {
    MoveTo(bounds->left + i*9, bounds->top);
    Line(0, 9);
  }
  for (i32 i = 0; i < value; i++) {
    Rect r;
    r.left = bounds->left + i*9 + 2;
    r.right = r.left + 6;
    r.top = bounds->top + 2;
    r.bottom = bounds->bottom-2;
    FillRect(&r, BLACK);
  }
}

static void EditCharges(Stat *stat, Rect *bounds)
{
  i32 originalValue = stat->value;
  SetColor(BLACK);

  Rect contentRect = *bounds;
  InsetRect(&contentRect, -2, -2);
  FrameRect(&contentRect);
  InsetRect(&contentRect, 1, 1);

  while (true) {
    VSync();
    u16 input = GetInput();

    if (KeyPressed(BTN_A)) {
      break;
    } else if (KeyPressed(BTN_B) || KeyPressed(BTN_SELECT)) {
      stat->value = originalValue;
      break;
    } else if (KeyPressed(BTN_LEFT) && stat->value > 0) {
      stat->value--;
    } else if (KeyPressed(BTN_RIGHT) && stat->value < stat->max) {
      stat->value++;
    }

    if (input & (BTN_LEFT | BTN_RIGHT)) {
      FillRect(&contentRect, WHITE);
      DrawCharges(stat->value, stat->max, bounds);
    }
  }

  FillRect(&contentRect, WHITE);
  DrawCharges(stat->value, stat->max, bounds);

  InsetRect(&contentRect, -1, -1);
  SetColor(WHITE);
  FrameRect(&contentRect);
}

static void PrintSimpleStat(Stat *stat, Rect *bounds)
{
  FontInfo info;
  GetFontInfo(&info);

  MoveTo(bounds->left, bounds->top + info.ascent);
  Print(stat->name);
  char numStr[16] = {0};
  NumToString(stat->value, numStr);
  MoveTo(bounds->right - TextWidth(numStr), bounds->top + info.ascent);
  Print(numStr);
}

static void PrintAbilityStat(Stat *stat, Rect *bounds)
{
  FontInfo info;
  GetFontInfo(&info);
  i32 mod = (stat->value - 10) / 2;

  MoveTo(bounds->left, bounds->top + info.ascent);
  Print(stat->name);
  char numStr[16] = {0};
  if (mod >= 0) {
    numStr[0] = '+';
    NumToString(mod, numStr+1);
  } else {
    NumToString(mod, numStr);
  }
  MoveTo(bounds->right - TextWidth(numStr), bounds->top + info.ascent);
  Print(numStr);

  ClearStr(numStr);
  NumToString(stat->value, numStr);
  MoveTo(bounds->right - TextWidth("00") - 8 - TextWidth(numStr), bounds->top + info.ascent);
  Print(numStr);
}

static void PrintChargeStat(Stat *stat, Rect *bounds)
{
  FontInfo info;
  GetFontInfo(&info);

  SetColor(BLACK);
  MoveTo(bounds->left, bounds->top + info.ascent);
  Print(stat->name);

  if (stat->max == 0) return;

  Rect outline = {bounds->right - 9*stat->max - 1, bounds->top, bounds->right, bounds->top + 10};
  DrawCharges(stat->value, stat->max, &outline);
}
