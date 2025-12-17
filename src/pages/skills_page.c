#include "pages/skills_page.h"
#include "kit/canvas.h"
#include "kit/text.h"
#include "views/num_stat_view.h"
#include "kit/mem.h"
#include "views/page.h"

void InitSkillsPage(Page *page)
{
  i16 lineHeight = LineHeight();
  Rect bounds;
  InitPage(page, "Skills");

  SetRect(&bounds, 8, 15, SCREEN_W/2-4, 15+lineHeight);
  NumStatView *acrobatics = Alloc(sizeof(NumStatView));
  InitNumStatView(acrobatics, &bounds, "Acrobatics", "Acrobatics", "");
  acrobatics->control.showSign = true;
  AddPageView(page, &acrobatics->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *animalhandling = Alloc(sizeof(NumStatView));
  InitNumStatView(animalhandling, &bounds, "Animal Handling", "AnimalHandling", "");
  animalhandling->control.showSign = true;
  AddPageView(page, &animalhandling->asView);
  LinkViewBelow(page, &acrobatics->asView, &animalhandling->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *arcana = Alloc(sizeof(NumStatView));
  InitNumStatView(arcana, &bounds, "Arcana", "Arcana", "");
  arcana->control.showSign = true;
  AddPageView(page, &arcana->asView);
  LinkViewBelow(page, &animalhandling->asView, &arcana->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *athletics = Alloc(sizeof(NumStatView));
  InitNumStatView(athletics, &bounds, "Athletics", "Athletics", "");
  athletics->control.showSign = true;
  AddPageView(page, &athletics->asView);
  LinkViewBelow(page, &arcana->asView, &athletics->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *deception = Alloc(sizeof(NumStatView));
  InitNumStatView(deception, &bounds, "Deception", "Deception", "");
  deception->control.showSign = true;
  AddPageView(page, &deception->asView);
  LinkViewBelow(page, &athletics->asView, &deception->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *history = Alloc(sizeof(NumStatView));
  InitNumStatView(history, &bounds, "History", "History", "");
  history->control.showSign = true;
  AddPageView(page, &history->asView);
  LinkViewBelow(page, &deception->asView, &history->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *insight = Alloc(sizeof(NumStatView));
  InitNumStatView(insight, &bounds, "Insight", "Insight", "");
  insight->control.showSign = true;
  AddPageView(page, &insight->asView);
  LinkViewBelow(page, &history->asView, &insight->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *intimidation = Alloc(sizeof(NumStatView));
  InitNumStatView(intimidation, &bounds, "Intimidation", "Intimidation", "");
  intimidation->control.showSign = true;
  AddPageView(page, &intimidation->asView);
  LinkViewBelow(page, &insight->asView, &intimidation->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *investigation = Alloc(sizeof(NumStatView));
  InitNumStatView(investigation, &bounds, "Investigation", "Investigation", "");
  investigation->control.showSign = true;
  AddPageView(page, &investigation->asView);
  LinkViewBelow(page, &intimidation->asView, &investigation->asView);

  SetRect(&bounds, SCREEN_W/2+8, 15, SCREEN_W-8, 15+lineHeight);
  NumStatView *medicine = Alloc(sizeof(NumStatView));
  InitNumStatView(medicine, &bounds, "Medicine", "Medicine", "");
  medicine->control.showSign = true;
  AddPageView(page, &medicine->asView);
  LinkViewBeside(page, &acrobatics->asView, &medicine->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *nature = Alloc(sizeof(NumStatView));
  InitNumStatView(nature, &bounds, "Nature", "Nature", "");
  nature->control.showSign = true;
  AddPageView(page, &nature->asView);
  LinkViewBelow(page, &medicine->asView, &nature->asView);
  LinkViewBeside(page, &animalhandling->asView, &nature->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *perception = Alloc(sizeof(NumStatView));
  InitNumStatView(perception, &bounds, "Perception", "Perception", "");
  perception->control.showSign = true;
  AddPageView(page, &perception->asView);
  LinkViewBelow(page, &nature->asView, &perception->asView);
  LinkViewBeside(page, &arcana->asView, &perception->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *performance = Alloc(sizeof(NumStatView));
  InitNumStatView(performance, &bounds, "Performance", "Performance", "");
  performance->control.showSign = true;
  AddPageView(page, &performance->asView);
  LinkViewBelow(page, &perception->asView, &performance->asView);
  LinkViewBeside(page, &athletics->asView, &performance->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *persuasion = Alloc(sizeof(NumStatView));
  InitNumStatView(persuasion, &bounds, "Persuasion", "Persuasion", "");
  persuasion->control.showSign = true;
  AddPageView(page, &persuasion->asView);
  LinkViewBelow(page, &performance->asView, &persuasion->asView);
  LinkViewBeside(page, &deception->asView, &persuasion->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *religion = Alloc(sizeof(NumStatView));
  InitNumStatView(religion, &bounds, "Religion", "Religion", "");
  religion->control.showSign = true;
  AddPageView(page, &religion->asView);
  LinkViewBelow(page, &persuasion->asView, &religion->asView);
  LinkViewBeside(page, &history->asView, &religion->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *sleightofhand = Alloc(sizeof(NumStatView));
  InitNumStatView(sleightofhand, &bounds, "Sleight Of Hand", "SleightOfHand", "");
  sleightofhand->control.showSign = true;
  AddPageView(page, &sleightofhand->asView);
  LinkViewBelow(page, &religion->asView, &sleightofhand->asView);
  LinkViewBeside(page, &insight->asView, &sleightofhand->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *stealth = Alloc(sizeof(NumStatView));
  InitNumStatView(stealth, &bounds, "Stealth", "Stealth", "");
  stealth->control.showSign = true;
  AddPageView(page, &stealth->asView);
  LinkViewBelow(page, &sleightofhand->asView, &stealth->asView);
  LinkViewBeside(page, &intimidation->asView, &stealth->asView);
  OffsetRect(&bounds, 0, lineHeight);
  NumStatView *survival = Alloc(sizeof(NumStatView));
  InitNumStatView(survival, &bounds, "Survival", "Survival", "");
  survival->control.showSign = true;
  AddPageView(page, &survival->asView);
  LinkViewBelow(page, &stealth->asView, &survival->asView);
  LinkViewBeside(page, &investigation->asView, &survival->asView);

  SelectView(page, &acrobatics->asView);
}
