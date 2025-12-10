#include "overview_page.h"
#include "elements.h"
#include "ui.h"

static void OverviewActivate(Page *page, bool active)
{
  SelectElement(page, page->selected);
}

void InitOverviewPage(Page *page)
{
  Rect bounds;
  FontInfo info;
  GetFontInfo(&info);
  u32 lineHeight = info.ascent + info.descent;

  InitPage(page, "Overview");
  page->activate = OverviewActivate;

  SetRect(&bounds, 8, 15, SCREEN_W/2-4, 15 + lineHeight);
  PageElement *hpElement = NewHPElement(&bounds, "HP");
  AddPageElement(page, hpElement);
  page->selected = hpElement;

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *acElement = NewStatElement(&bounds, "AC", "AC", "ArmorMod");
  AddPageElement(page, acElement);
  AddElementBelow(hpElement, acElement);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *speedEl = NewStatElement(&bounds, "Speed", "Speed", 0);
  AddPageElement(page, speedEl);
  AddElementBelow(acElement, speedEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *spellSaveEl = NewStatElement(&bounds, "Spell Save DC", "SpellSaveDC", 0);
  AddPageElement(page, spellSaveEl);
  AddElementBelow(speedEl, spellSaveEl);

  OffsetRect(&bounds, 0, lineHeight+8);
  PageElement *strEl = NewAbilityStatElement(&bounds, "STR", "STR", "ModSTR");
  AddPageElement(page, strEl);
  AddElementBelow(spellSaveEl, strEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *dexEl = NewAbilityStatElement(&bounds, "DEX", "DEX", "ModDEX");
  AddPageElement(page, dexEl);
  AddElementBelow(strEl, dexEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *conEl = NewAbilityStatElement(&bounds, "CON", "CON", "ModCON");
  AddPageElement(page, conEl);
  AddElementBelow(dexEl, conEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *intEl = NewAbilityStatElement(&bounds, "INT", "INT", "ModINT");
  AddPageElement(page, intEl);
  AddElementBelow(conEl, intEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *wisEl = NewAbilityStatElement(&bounds, "WIS", "WIS", "ModWIS");
  AddPageElement(page, wisEl);
  AddElementBelow(intEl, wisEl);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *chaEl = NewAbilityStatElement(&bounds, "CHA", "CHA", "ModCHA");
  AddPageElement(page, chaEl);
  AddElementBelow(wisEl, chaEl);

  SetRect(&bounds, SCREEN_W - SCREEN_W/4 - TextWidth("Spell Slots")/2, 15, SCREEN_W-4, 15 + lineHeight);
  PageElement *slotsLabel = NewLabel(&bounds, "Spell Slots");
  AddPageElement(page, slotsLabel);

  SetRect(&bounds, SCREEN_W/2+10, 15 + lineHeight + 5, SCREEN_W-4, 15+2*lineHeight + 5);
  PageElement *l1slots = NewChargeElement(&bounds, "L1", "L1SlotsUsed", "L1Slots");
  AddPageElement(page, l1slots);
  AddElementBeside(acElement, l1slots);
  hpElement->right = l1slots;

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *l2slots = NewChargeElement(&bounds, "L2", "L2SlotsUsed", "L2Slots");
  AddPageElement(page, l2slots);
  AddElementBelow(l1slots, l2slots);
  AddElementBeside(speedEl, l2slots);

  OffsetRect(&bounds, 0, lineHeight);
  PageElement *l3slots = NewChargeElement(&bounds, "L3", "L3SlotsUsed", "L3Slots");
  AddPageElement(page, l3slots);
  AddElementBelow(l2slots, l3slots);
  AddElementBeside(spellSaveEl, l3slots);

  strEl->right = l3slots;
  dexEl->right = l3slots;
  conEl->right = l3slots;
  intEl->right = l3slots;
  wisEl->right = l3slots;
  chaEl->right = l3slots;

  SelectElement(page, hpElement);
}
