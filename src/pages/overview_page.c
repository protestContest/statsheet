#include "pages/overview_page.h"
#include "kit/canvas.h"
#include "kit/text.h"
#include "views/charge_stat_view.h"
#include "views/num_stat_view.h"
#include "kit/mem.h"
#include "views/page.h"
#include "views/label.h"

void InitOverviewPage(Page *page)
{
  FontInfo info;
  GetFontInfo(&info);
  i16 lineHeight = info.ascent + info.descent;
  Rect bounds;
  InitPage(page, "Overview");

  NumStatView *hp = Alloc(sizeof(NumStatView));

  SetRect(&bounds, 8, 15, SCREEN_W/2-4 - TextWidth(" / 00"), 26);
  InitNumStatView(hp, &bounds, "HP", "HP", "");
  AddPageView(page, &hp->asView);

  NumStatView *maxHp = Alloc(sizeof(NumStatView));
  SetRect(&bounds, SCREEN_W/2-4 - TextWidth("/00"), 15, SCREEN_W/2-4, 26);
  InitNumStatView(maxHp, &bounds, "/", "MaxHP", "TmpMaxHP");
  AddPageView(page, &maxHp->asView);
  LinkViewBeside(page, &hp->asView, &maxHp->asView);

  SetRect(&bounds, 8, 15+lineHeight, SCREEN_W/2-4, 15+2*lineHeight);
  NumStatView *ac = Alloc(sizeof(NumStatView));
  InitNumStatView(ac, &bounds, "AC", "AC", "ArmorMod");
  AddPageView(page, &ac->asView);
  LinkViewBelow(page, &hp->asView, &ac->asView);
  LinkViewDown(page, &maxHp->asView, &ac->asView);

  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *speed = Alloc(sizeof(NumStatView));
  InitNumStatView(speed, &bounds, "Speed", "Speed", 0);
  AddPageView(page, &speed->asView);
  LinkViewBelow(page, &ac->asView, &speed->asView);

  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *spellSave = Alloc(sizeof(NumStatView));
  InitNumStatView(spellSave, &bounds, "Spell Save DC", "SpellSaveDC", 0);
  AddPageView(page, &spellSave->asView);
  LinkViewBelow(page, &speed->asView, &spellSave->asView);

  bounds.right -= 30;
  OffsetRect(&bounds, 0, lineHeight + 8);
  NumStatView *str = Alloc(sizeof(NumStatView));
  InitNumStatView(str, &bounds, "STR", "STR", 0);
  AddPageView(page, &str->asView);
  LinkViewBelow(page, &spellSave->asView, &str->asView);

  Rect modBounds = bounds;
  modBounds.right += 30;
  modBounds.left = modBounds.right - TextWidth("+00");
  NumStatView *strMod = Alloc(sizeof(NumStatView));
  InitNumStatView(strMod, &modBounds, "", "ModSTR", 0);
  strMod->control.showSign = true;
  AddPageView(page, &strMod->asView);
  LinkViewBeside(page, &str->asView, &strMod->asView);
  LinkViewUp(page, &strMod->asView, &spellSave->asView);

  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *dex = Alloc(sizeof(NumStatView));
  InitNumStatView(dex, &bounds, "DEX", "DEX", 0);
  AddPageView(page, &dex->asView);
  LinkViewBelow(page, &str->asView, &dex->asView);

  OffsetRect(&modBounds, 0, lineHeight);
  NumStatView *dexMod = Alloc(sizeof(NumStatView));
  InitNumStatView(dexMod, &modBounds, "", "ModDEX", 0);
  dexMod->control.showSign = true;
  AddPageView(page, &dexMod->asView);
  LinkViewBelow(page, &strMod->asView, &dexMod->asView);
  LinkViewBeside(page, &dex->asView, &dexMod->asView);

  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *con = Alloc(sizeof(NumStatView));
  InitNumStatView(con, &bounds, "CON", "CON", 0);
  AddPageView(page, &con->asView);
  LinkViewBelow(page, &dex->asView, &con->asView);

  OffsetRect(&modBounds, 0, lineHeight);
  NumStatView *conMod = Alloc(sizeof(NumStatView));
  InitNumStatView(conMod, &modBounds, "", "ModCON", 0);
  conMod->control.showSign = true;
  AddPageView(page, &conMod->asView);
  LinkViewBelow(page, &dexMod->asView, &conMod->asView);
  LinkViewBeside(page, &con->asView, &conMod->asView);

  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *intl = Alloc(sizeof(NumStatView));
  InitNumStatView(intl, &bounds, "INT", "INT", 0);
  AddPageView(page, &intl->asView);
  LinkViewBelow(page, &con->asView, &intl->asView);

  OffsetRect(&modBounds, 0, lineHeight);
  NumStatView *intMod = Alloc(sizeof(NumStatView));
  InitNumStatView(intMod, &modBounds, "", "ModINT", 0);
  intMod->control.showSign = true;
  AddPageView(page, &intMod->asView);
  LinkViewBelow(page, &conMod->asView, &intMod->asView);
  LinkViewBeside(page, &intl->asView, &intMod->asView);

  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *wis = Alloc(sizeof(NumStatView));
  InitNumStatView(wis, &bounds, "WIS", "WIS", 0);
  AddPageView(page, &wis->asView);
  LinkViewBelow(page, &intl->asView, &wis->asView);

  OffsetRect(&modBounds, 0, lineHeight);
  NumStatView *wisMod = Alloc(sizeof(NumStatView));
  InitNumStatView(wisMod, &modBounds, "", "ModWIS", 0);
  wisMod->control.showSign = true;
  AddPageView(page, &wisMod->asView);
  LinkViewBelow(page, &intMod->asView, &wisMod->asView);
  LinkViewBeside(page, &wis->asView, &wisMod->asView);

  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *cha = Alloc(sizeof(NumStatView));
  InitNumStatView(cha, &bounds, "CHA", "CHA", 0);
  AddPageView(page, &cha->asView);
  LinkViewBelow(page, &wis->asView, &cha->asView);

  OffsetRect(&modBounds, 0, lineHeight);
  NumStatView *chaMod = Alloc(sizeof(NumStatView));
  InitNumStatView(chaMod, &modBounds, "", "ModCHA", 0);
  chaMod->control.showSign = true;
  AddPageView(page, &chaMod->asView);
  LinkViewBelow(page, &wisMod->asView, &chaMod->asView);
  LinkViewBeside(page, &cha->asView, &chaMod->asView);

  Label *l = Alloc(sizeof(Label));
  SetRect(&bounds, SCREEN_W*3/4-TextWidth("Spells")/2, 15, SCREEN_W*3/4+TextWidth("Spells")/2, 15 + LineHeight());
  InitLabel(l, &bounds, "Spells");
  AddPageView(page, &l->asView);

  SetRect(&bounds, SCREEN_W/2+8, 15+lineHeight+4, SCREEN_W-4, 15+2*lineHeight);
  ChargeStatView *l1 = Alloc(sizeof(ChargeStatView));
  InitChargeStatView(l1, &bounds, "L1", "L1SlotsUsed", "L1Slots");
  AddPageView(page, &l1->asView);
  LinkViewBeside(page, &ac->asView, &l1->asView);
  LinkViewRight(page, &maxHp->asView, &l1->asView);

  OffsetRect(&bounds, 0, lineHeight);
  ChargeStatView *l2 = Alloc(sizeof(ChargeStatView));
  InitChargeStatView(l2, &bounds, "L2", "L2SlotsUsed", "L2Slots");
  AddPageView(page, &l2->asView);
  LinkViewBelow(page, &l1->asView, &l2->asView);
  LinkViewBeside(page, &speed->asView, &l2->asView);

  OffsetRect(&bounds, 0, lineHeight);
  ChargeStatView *l3 = Alloc(sizeof(ChargeStatView));
  InitChargeStatView(l3, &bounds, "L3", "L3SlotsUsed", "L3Slots");
  AddPageView(page, &l3->asView);
  LinkViewBelow(page, &l2->asView, &l3->asView);
  LinkViewBeside(page, &spellSave->asView, &l3->asView);

  LinkViewRight(page, &strMod->asView, &l3->asView);
  LinkViewRight(page, &dexMod->asView, &l3->asView);
  LinkViewRight(page, &conMod->asView, &l3->asView);
  LinkViewRight(page, &intMod->asView, &l3->asView);
  LinkViewRight(page, &wisMod->asView, &l3->asView);
  LinkViewRight(page, &chaMod->asView, &l3->asView);

  SelectView(page, &hp->asView);
}
