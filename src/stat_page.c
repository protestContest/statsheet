#include "stat_page.h"
#include "stat.h"
#include "stat_list.h"
#include "label.h"

void InitStatPage(Page *page)
{
  InitPage(page, "Stats");

  char *stats[] = {
    "HP",
    "Hit Die",
    "AC",
    "Speed",
    "Spell Save DC",
    "-",
    "STR",
    "DEX",
    "CON",
    "INT",
    "WIS",
    "CHA",
  };

  StatList *list = Alloc(sizeof(StatList));
  Rect listRect = {0, 15, SCREEN_W/2 - 4, SCREEN_H};
  InitStatList(list, &listRect, ArrayCount(stats), stats);
  AddPageElement(page, (PageElement*)list);

  Label *slotLabel = Alloc(sizeof(Label));
  Rect slotLabelRect = {SCREEN_W - SCREEN_W/4 - TextWidth("Spell Slots")/2, 15, SCREEN_W, 26};
  InitLabel(slotLabel, &slotLabelRect, "Spell Slots");
  AddPageElement(page, (PageElement*)slotLabel);

  char *slots[] = {
    "L1",
    "L2",
    "L3"
  };
  StatList *slotList = Alloc(sizeof(StatList));
  Rect slotListRect = {SCREEN_W/2 + 4, 30, SCREEN_W - 4, SCREEN_H};
  InitStatList(slotList, &slotListRect, ArrayCount(slots), slots);
  AddPageElement(page, (PageElement*)slotList);
}
