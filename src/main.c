#include "kit.h"
#include "stat.h"
#include "ui.h"
#include "overview_page.h"

void main(void)
{
  EnableDebug();
  InitKit();
  InitSprites("sprites.tga");
  InitUI();

  SetFont("Geneva");

  InitStats();

  Page overviewPage;
  InitOverviewPage(&overviewPage);

  DrawPage(&overviewPage);

  while (true) {
    VSync();
    u16 input = GetInput();
    OnPageInput(&overviewPage, input);
  }
}
