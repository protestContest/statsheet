#include "kit/debug.h"
#include "kit/init.h"
#include "kit/sprite.h"
#include "kit/text.h"
#include "stat.h"
#include "ui.h"
#include "views/page.h"
#include "views/page_list.h"
#include "pages/overview_page.h"

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

  // Page skillsPage;
  // InitPage(&skillsPage, "Skills");
  // AddPage(&pages, &skillsPage);

  Run(&pages.asView);
}
