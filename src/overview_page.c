#include "overview_page.h"
#include "elements.h"
#include "page.h"

static void OverviewPageActivate(View *view, bool active)
{
  Page *page = (Page*)view;
  ActivatePage(view, active);
  if (active) {
    SelectElement(page, page->selected);
  }
}

Page *NewOverviewPage(void)
{
  Rect bounds;
  FontInfo info;
  GetFontInfo(&info);
  u32 lineHeight = info.ascent + info.descent;

  Page *page = Alloc(sizeof(Page));

  InitPage(page, "Overview");
  page->asView.activate = OverviewPageActivate;

  SetRect(&bounds, 8, 15, SCREEN_W/2-4, 15 + lineHeight);
  PageElement *hpElement = NewHPElement(&bounds, "HP");
  AddPageElement(page, hpElement);
  page->selected = hpElement;

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *acElement = NewStatElement(&bounds, "AC", "AC", "ArmorMod");
  AddPageElement(page, acElement);
  LinkElementBelow(hpElement, acElement);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *speedEl = NewStatElement(&bounds, "Speed", "Speed", 0);
  AddPageElement(page, speedEl);
  LinkElementBelow(acElement, speedEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *spellSaveEl = NewStatElement(&bounds, "Spell Save DC", "SpellSaveDC", 0);
  AddPageElement(page, spellSaveEl);
  LinkElementBelow(speedEl, spellSaveEl);

  OffsetRect(&bounds, 0, lineHeight+8);
  PageElement *strEl = NewAbilityStatElement(&bounds, "STR", "STR", "ModSTR");
  AddPageElement(page, strEl);
  LinkElementBelow(spellSaveEl, strEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *dexEl = NewAbilityStatElement(&bounds, "DEX", "DEX", "ModDEX");
  AddPageElement(page, dexEl);
  LinkElementBelow(strEl, dexEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *conEl = NewAbilityStatElement(&bounds, "CON", "CON", "ModCON");
  AddPageElement(page, conEl);
  LinkElementBelow(dexEl, conEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *intEl = NewAbilityStatElement(&bounds, "INT", "INT", "ModINT");
  AddPageElement(page, intEl);
  LinkElementBelow(conEl, intEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *wisEl = NewAbilityStatElement(&bounds, "WIS", "WIS", "ModWIS");
  AddPageElement(page, wisEl);
  LinkElementBelow(intEl, wisEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *chaEl = NewAbilityStatElement(&bounds, "CHA", "CHA", "ModCHA");
  AddPageElement(page, chaEl);
  LinkElementBelow(wisEl, chaEl);

  SetRect(&bounds, SCREEN_W - SCREEN_W/4 - TextWidth("Spell Slots")/2, 15, SCREEN_W-4, 15 + lineHeight);
  PageElement *slotsLabel = NewLabel(&bounds, "Spell Slots");
  AddPageElement(page, slotsLabel);

  SetRect(&bounds, SCREEN_W/2+8, 15 + lineHeight + 5, SCREEN_W-8, 15+2*lineHeight + 5);
  PageElement *l1slots = NewChargeElement(&bounds, "L1", "L1SlotsUsed", "L1Slots");
  AddPageElement(page, l1slots);
  LinkElementBeside(acElement, l1slots);
  hpElement->right = l1slots;

  OffsetRect(&bounds, 0, lineHeight+1);
  PageElement *l2slots = NewChargeElement(&bounds, "L2", "L2SlotsUsed", "L2Slots");
  AddPageElement(page, l2slots);
  LinkElementBelow(l1slots, l2slots);
  LinkElementBeside(speedEl, l2slots);

  OffsetRect(&bounds, 0, lineHeight+1);
  PageElement *l3slots = NewChargeElement(&bounds, "L3", "L3SlotsUsed", "L3Slots");
  AddPageElement(page, l3slots);
  LinkElementBelow(l2slots, l3slots);
  LinkElementBeside(spellSaveEl, l3slots);

  OffsetRect(&bounds, 0, lineHeight+15);
  PageElement *inspa = NewChargeElement(&bounds, "Inspiration", "InspirationUsed", "InspirationMax");
  AddPageElement(page, inspa);
  LinkElementBelow(l3slots, inspa);
  LinkElementBeside(dexEl, inspa);

  strEl->right = l3slots;
  conEl->right = inspa;
  intEl->right = inspa;
  wisEl->right = inspa;
  chaEl->right = inspa;

  SelectElement(page, hpElement);

  return page;
}
