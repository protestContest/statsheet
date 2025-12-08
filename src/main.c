#include "kit.h"
#include "stat.h"
#include "cursor.h"
#include "ui.h"
#include "stat_page.h"
#include "skills_page.h"

Stat stats[] = {
  {simpleStat, "HP", 38, 0, 0, 0},
  {chargeStat, "Hit Die", 0, 0, 6, 0},
  {simpleStat, "AC", 11, 0, 0, 0},
  {simpleStat, "Speed", 30, 0, 0, 0},
  {simpleStat, "Spell Save DC", 15, 0, 0, 0},
  {abilityStat, "STR", 9, 0, 0, 0},
  {abilityStat, "DEX", 12, 0, 0, 0},
  {abilityStat, "CON", 14, 0, 0, 0},
  {abilityStat, "INT", 18, 0, 0, 0},
  {abilityStat, "WIS", 14, 0, 0, 0},
  {abilityStat, "CHA", 10, 0, 0, 0},
  {chargeStat, "L1", 2, 0, 4, 0},
  {chargeStat, "L2", 0, 0, 3, 0},
  {chargeStat, "L3", 0, 0, 3, 0},
  {modStat, "Acrobatics", 0, 0, 0, 0},
  {modStat, "Animal Handling", 0, 0, 0, 0},
  {modStat, "Arcana", 0, 0, 0, 0},
  {modStat, "Athletics", 0, 0, 0, 0},
  {modStat, "Deception", 0, 0, 0, 0},
  {modStat, "History", 0, 0, 0, 0},
  {modStat, "Insight", 0, 0, 0, 0},
  {modStat, "Intimidation", 0, 0, 0, 0},
  {modStat, "Investigation", 0, 0, 0, 0},
  {modStat, "Medicine", 0, 0, 0, 0},
  {modStat, "Nature", 0, 0, 0, 0},
  {modStat, "Perception", 0, 0, 0, 0},
  {modStat, "Performance", 0, 0, 0, 0},
  {modStat, "Persuasion", 0, 0, 0, 0},
  {modStat, "Religion", 0, 0, 0, 0},
  {modStat, "Sleight of Hand", 0, 0, 0, 0},
  {modStat, "Stealth", 0, 0, 0, 0},
  {modStat, "Survival", 0, 0, 0, 0},
};

Page pages[2];
u32 curPage = 0;

void main(void)
{
  InitKit();
  InitSprites("sprites.tga");
  InitUI();
  InitCursor();

  SetFont("Geneva");

  for (u32 i = 0; i < ArrayCount(stats); i++) {
    AddStat(&stats[i], stats[i].name, stats[i].type, stats[i].value);
  }

  InitStatPage(&pages[0]);
  InitSkillsPage(&pages[1]);

  DrawPage(&pages[curPage]);

  while (true) {
    VSync();
    u16 input = GetInput();
    if (KeyPressed(BTN_L)) {
      if (curPage == 0) {
        curPage = ArrayCount(pages)-1;
      } else {
        curPage--;
      }
      HideCursor();
      DrawPage(&pages[curPage]);
    } else if (KeyPressed(BTN_R)) {
      curPage = (curPage+1) % ArrayCount(pages);
      HideCursor();
      DrawPage(&pages[curPage]);
    } else {
      OnPageInput(&pages[curPage], input);
    }
  }
}
