// #include "kit/canvas.h"
// #include "skills_page.h"
// #include "elements.h"
// #include "page_element.h"

// static Page skillsPage;

// static void SkillsPageActivate(View *view, bool active)
// {
//   Page *page = (Page*)view;
//   ActivatePage(view, active);
//   if (active) {
//     SelectElement(page, page->selected);
//   }
// }

// Page *InitSkillsPage(void)
// {
//   Rect bounds;
//   FontInfo info;
//   GetFontInfo(&info);
//   u32 lineHeight = info.ascent + info.descent;

//   Page *page = &skillsPage;
//   InitPage(page, "Skills");
//   page->asView.activate = SkillsPageActivate;

//   SetRect(&bounds, 8, 15, SCREEN_W/2-4, 15 + lineHeight);

//   PageElement *acrobaticsEl = NewModElement(&bounds, "Acrobatics", "Acrobatics", 0);
//   AddPageElement(page, acrobaticsEl);
//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *animalHandlingEl = NewModElement(&bounds, "Animal Handling", "AnimalHandling", 0);
//   AddPageElement(page, animalHandlingEl);
//   LinkElementBelow(acrobaticsEl, animalHandlingEl);
//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *arcanaEl = NewModElement(&bounds, "Arcana", "Arcana", 0);
//   AddPageElement(page, arcanaEl);
//   LinkElementBelow(animalHandlingEl, arcanaEl);
//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *athleticsEl = NewModElement(&bounds, "Athletics", "Athletics", 0);
//   AddPageElement(page, athleticsEl);
//   LinkElementBelow(arcanaEl, athleticsEl);
//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *deceptionEl = NewModElement(&bounds, "Deception", "Deception", 0);
//   AddPageElement(page, deceptionEl);
//   LinkElementBelow(athleticsEl, deceptionEl);
//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *historyEl = NewModElement(&bounds, "History", "History", 0);
//   AddPageElement(page, historyEl);
//   LinkElementBelow(deceptionEl, historyEl);
//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *insightEl = NewModElement(&bounds, "Insight", "Insight", 0);
//   AddPageElement(page, insightEl);
//   LinkElementBelow(historyEl, insightEl);
//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *intimidationEl = NewModElement(&bounds, "Intimidation", "Intimidation", 0);
//   AddPageElement(page, intimidationEl);
//   LinkElementBelow(insightEl, intimidationEl);
//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *investigationEl = NewModElement(&bounds, "Investigation", "Investigation", 0);
//   AddPageElement(page, investigationEl);
//   LinkElementBelow(intimidationEl, investigationEl);
//   LinkElementBelow(investigationEl, acrobaticsEl);

//   SetRect(&bounds, SCREEN_W/2+8, 15, SCREEN_W-8, 15 + lineHeight);
//   PageElement *medicineEl = NewModElement(&bounds, "Medicine", "Medicine", 0);
//   AddPageElement(page, medicineEl);
//   LinkElementBeside(acrobaticsEl, medicineEl);

//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *natureEl = NewModElement(&bounds, "Nature", "Nature", 0);
//   AddPageElement(page, natureEl);
//   LinkElementBelow(medicineEl, natureEl);
//   LinkElementBeside(animalHandlingEl, natureEl);

//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *perceptionEl = NewModElement(&bounds, "Perception", "Perception", 0);
//   AddPageElement(page, perceptionEl);
//   LinkElementBelow(natureEl, perceptionEl);
//   LinkElementBeside(arcanaEl, perceptionEl);

//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *performanceEl = NewModElement(&bounds, "Performance", "Performance", 0);
//   AddPageElement(page, performanceEl);
//   LinkElementBelow(perceptionEl, performanceEl);
//   LinkElementBeside(athleticsEl, performanceEl);

//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *persuasionEl = NewModElement(&bounds, "Persuasion", "Persuasion", 0);
//   AddPageElement(page, persuasionEl);
//   LinkElementBelow(performanceEl, persuasionEl);
//   LinkElementBeside(deceptionEl, persuasionEl);

//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *religionEl = NewModElement(&bounds, "Religion", "Religion", 0);
//   AddPageElement(page, religionEl);
//   LinkElementBelow(persuasionEl, religionEl);
//   LinkElementBeside(historyEl, religionEl);

//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *sleightOfHandEl = NewModElement(&bounds, "Sleight Of Hand", "SleightOfHand", 0);
//   AddPageElement(page, sleightOfHandEl);
//   LinkElementBelow(religionEl, sleightOfHandEl);
//   LinkElementBeside(insightEl, sleightOfHandEl);

//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *stealthEl = NewModElement(&bounds, "Stealth", "Stealth", 0);
//   AddPageElement(page, stealthEl);
//   LinkElementBelow(sleightOfHandEl, stealthEl);
//   LinkElementBeside(intimidationEl, stealthEl);

//   OffsetRect(&bounds, 0, lineHeight);
//   PageElement *survivalEl = NewModElement(&bounds, "Survival", "Survival", 0);
//   AddPageElement(page, survivalEl);
//   LinkElementBelow(stealthEl, survivalEl);
//   LinkElementBelow(survivalEl, medicineEl);
//   LinkElementBeside(investigationEl, survivalEl);

//   SelectElement(page, acrobaticsEl);

//   return page;
// }
