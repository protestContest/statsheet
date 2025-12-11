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
  pages[curPage].asView.activate(&pages[curPage].asView, true);

  while (true) {
    VSync();
    u16 input = GetInput();
    if (KeyPressed(BTN_L)) {
      pages[curPage].asView.activate(&pages[curPage].asView, false);
      curPage = (curPage == 0) ? ArrayCount(pages)-1 : curPage-1;
      pages[curPage].asView.activate(&pages[curPage].asView, true);
    } else if (KeyPressed(BTN_R)) {
      pages[curPage].asView.activate(&pages[curPage].asView, false);
      curPage = (curPage + 1) % ArrayCount(pages);
      pages[curPage].asView.activate(&pages[curPage].asView, true);
    } else {
      pages[curPage].asView.onInput(&pages[curPage].asView, input);
    }
  }
}
