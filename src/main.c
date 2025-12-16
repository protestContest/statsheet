#include "kit/debug.h"
#include "kit/init.h"
#include "kit/sprite.h"
#include "kit/text.h"
#include "pages/overview_page.h"
#include "pages/skills_page.h"
#include "stat.h"
#include "ui.h"
#include "views/page_list.h"

int main(void)
{
  EnableDebug();
  InitKit();
  InitSprites("sprites.tga");
  InitUI();

  SetFont("Geneva");

  InitStats();

  PageList pages;
  InitPageList(&pages);

  Page overviewPage;
  InitOverviewPage(&overviewPage);
  AddPage(&pages, &overviewPage);

  Page skillsPage;
  InitSkillsPage(&skillsPage);
  AddPage(&pages, &skillsPage);

  Run(&pages.asView);
}
