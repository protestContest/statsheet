#include "kit.h"
#include "stat.h"
#include "ui.h"
#include "overview_page.h"
#include "dice_page.h"

Page pages[2];
u32 curPage = 0;

int main(void)
{
  EnableDebug();
  InitKit();
  InitSprites("sprites.tga");
  InitUI();

  SetFont("Geneva");

  InitStats();

  InitOverviewPage(&pages[0]);
  InitDicePage(&pages[1]);

  HideAllObjs();
  ActivatePage(&pages[curPage], true);

  while (true) {
    VSync();
    u16 input = GetInput();
    if (KeyPressed(BTN_L)) {
      ActivatePage(&pages[curPage], false);
      curPage = (curPage == 0) ? ArrayCount(pages)-1 : curPage-1;
      ActivatePage(&pages[curPage], true);
    } else if (KeyPressed(BTN_R)) {
      ActivatePage(&pages[curPage], false);
      curPage = (curPage + 1) % ArrayCount(pages);
      ActivatePage(&pages[curPage], true);
    } else {
      OnPageInput(&pages[curPage], input);
    }
  }
}
