#include "kit.h"
#include "menu.h"
#include "stat.h"
#include "ui.h"
#include "overview_page.h"
#include "skills_page.h"
#include "spells_page.h"
#include "dice_page.h"

Menu startMenu;

int main(void)
{
  EnableDebug();
  InitKit();
  InitSprites("sprites.tga");
  InitUI();

  SetFont("Geneva");

  InitStats();

  InitMenu(&startMenu);
  AddMenuItem(&startMenu, "Short Rest", 0, 0);
  AddMenuItem(&startMenu, "Long Rest", 0, 0);

  Page *overview = InitOverviewPage();
  Page *skills = InitSkillsPage();
  Page *spells = InitSpellsPage();
  Page *dice = InitDicePage();

  LinkPage(overview, skills);
  LinkPage(skills, spells);
  LinkPage(spells, dice);
  LinkPage(dice, overview);

  SelectPage(overview);

  while (true) {
    VSync();
    u16 input = GetInput();
    if (KeyPressed(BTN_START)) {
      ShowMenu(&startMenu);
    } else if (KeyPressed(BTN_L)) {
      PrevPage();
    } else if (KeyPressed(BTN_R)) {
      NextPage();
    } else {
      HandleInput(input);
    }
  }
}
