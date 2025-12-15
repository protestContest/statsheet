#include "kit/debug.h"
#include "kit/init.h"
#include "kit/sprite.h"
#include "kit/text.h"
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

  PageList *pages = BuildPageList("Pages");

  Run(&pages->asView);
}
