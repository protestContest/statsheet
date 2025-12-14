#include "pages/overview_page.h"
#include "kit/canvas.h"
#include "views/charge_stat_view.h"
#include "views/num_stat_view.h"
#include "kit/mem.h"

void InitOverviewPage(Page *page)
{
  InitPage(page, "Overview");

  NumStatView *ac = Alloc(sizeof(NumStatView));
  Rect bounds = {8, 50, 80, 61};
  InitNumStatView(ac, &bounds, "AC", "AC", "ArmorMod");
  AddPageView(page, &ac->asView);

  ChargeStatView *l1 = Alloc(sizeof(ChargeStatView));
  SetRect(&bounds, 8, 65, 80, 76);
  InitChargeStatView(l1, &bounds, "L1", "L1SlotsUsed", "L1Slots");
  AddPageView(page, &l1->asView);
  LinkViewBelow(page, &ac->asView, &l1->asView);

  SelectView(page, &ac->asView);
}
