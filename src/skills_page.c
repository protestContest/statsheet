#include "skills_page.h"
#include "stat.h"
#include "stat_list.h"
#include "label.h"

void InitSkillsPage(Page *page)
{
  InitPage(page, "Skills");

  char *leftStats[] = {
    "Acrobatics",
    "Animal Handling",
    "Arcana",
    "Athletics",
    "Deception",
    "History",
    "Insight",
    "Intimidation",
    "Investigation"
  };

  char *rightStats[] = {
    "Medicine",
    "Nature",
    "Perception",
    "Performance",
    "Persuasion",
    "Religion",
    "Sleight of Hand",
    "Stealth",
    "Survival"
  };

  StatList *left = Alloc(sizeof(StatList));
  Rect leftRect = {0, 15, SCREEN_W/2 - 4, SCREEN_H};
  InitStatList(left, &leftRect, ArrayCount(leftStats), leftStats);
  AddPageElement(page, (PageElement*)left);

  StatList *right = Alloc(sizeof(StatList));
  Rect rightRect = {SCREEN_W/2 + 4, 15, SCREEN_W - 4, SCREEN_H};
  InitStatList(right, &rightRect, ArrayCount(rightStats), rightStats);
  AddPageElement(page, (PageElement*)right);
}
