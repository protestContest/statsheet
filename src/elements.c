// #include "elements.h"
// #include "page.h"
// #include "page_element.h"
// #include "elements.h"
// #include "ui.h"
// #include "dice.h"

// static void DrawLabel(View *el)
// {
//   Label *label = (Label*)el;
//   FontInfo info;
//   GetFontInfo(&info);
//   MoveTo(el->bounds.left, el->bounds.top + info.ascent);
//   SetColor(BLACK);
//   Print(label->title);
//   MoveTo(el->bounds.left, el->bounds.top + info.ascent + 1);
//   Line(TextWidth(label->title), 0);
// }

// PageElement *NewLabel(Rect *bounds, char *title)
// {
//   Label *label = Alloc(sizeof(Label));
//   InitPageElement(&label->asElement, bounds, DrawLabel, 0);
//   label->title = title;
//   return (PageElement*)label;
// }

// static void DrawButtonUp(Rect *bounds)
// {
//   Rect r = *bounds;
//   SetColor(RGB(48, 48, 48));
//   MoveTo(r.left, r.bottom-3);
//   LineTo(r.left, r.top+2);
//   LineTo(r.left+2, r.top);
//   LineTo(r.right-3, r.top);
//   LineTo(r.right-1, r.top+2);
//   LineTo(r.right-1, r.bottom-3);
//   SetColor(BLACK);
//   LineTo(r.right-3, r.bottom-1);
//   LineTo(r.left+2, r.bottom-1);
//   InsetRect(&r, 1, 1);
//   SetColor(RGB(128, 128, 128));
//   MoveTo(r.left, r.bottom-2);
//   LineTo(r.left, r.top+1);
//   LineTo(r.left+1, r.top);
//   LineTo(r.right-2, r.top);
//   SetColor(RGB(48, 48, 48));
//   LineTo(r.right-1, r.top+1);
//   LineTo(r.right-1, r.bottom-2);
//   LineTo(r.right-2, r.bottom-1);
//   LineTo(r.left, r.bottom-1);
//   InsetRect(&r, 1, 1);
//   FillRect(&r, RGB(90, 90, 90));
// }

// static void DrawButtonDown(Rect *bounds)
// {
//   Rect r = *bounds;
//   SetColor(RGB(48, 48, 48));
//   MoveTo(r.left, r.bottom-3);
//   LineTo(r.left, r.top+2);
//   LineTo(r.left+2, r.top);
//   LineTo(r.right-3, r.top);
//   LineTo(r.right-1, r.top+2);
//   LineTo(r.right-1, r.bottom-2);
//   LineTo(r.right-3, r.bottom-1);
//   LineTo(r.left+2, r.bottom-1);
//   SetColor(BLACK);
//   MoveTo(r.left+1, r.bottom-2);
//   LineTo(r.left+1, r.top+2);
//   LineTo(r.left+2, r.top+1);
//   LineTo(r.right-3, r.top+1);
//   SetColor(RGB(128, 128, 128));
//   MoveTo(r.right-2, r.top+2);
//   LineTo(r.right-2, r.bottom-3);
//   LineTo(r.right-3, r.bottom-2);
//   LineTo(r.left+2, r.bottom-2);
//   InsetRect(&r, 2, 2);
//   FillRect(&r, RGB(90, 90, 90));
// }

// static void DrawButton(View *el)
// {
//   Button *button = (Button*)el;
//   if (button->active) {
//     DrawButtonDown(&el->bounds);
//   } else {
//     DrawButtonUp(&el->bounds);
//   }

//   FontInfo info;
//   GetFontInfo(&info);
//   MoveTo((el->bounds.right+el->bounds.left)/2 - TextWidth(button->title)/2,
//          (el->bounds.bottom+el->bounds.top)/2 - (info.ascent+info.descent)/2 + info.ascent);
//   SetColor(WHITE);
//   Print(button->title);
// }

// void ButtonInput(View *el, u16 input)
// {
//   Button *button = (Button*)el;
//   button->active = (input & BTN_A) ? true : false;

//   if (KeyPressed(BTN_A) || KeyReleased(BTN_A)) {
//     EraseRect(&el->bounds);
//     DrawButton(el);
//   }
// }

// PageElement *NewButton(Rect *bounds, char *title)
// {
//   Button *button = Alloc(sizeof(Button));
//   InitPageElement(&button->asElement, bounds, DrawButton, ButtonInput);
//   button->title = title;
//   button->active = false;
//   return (PageElement*)button;
// }

// static void DrawHPElement(View *el)
// {
//   MaxStatElement *hpEl = (MaxStatElement*)el;
//   FontInfo info;
//   GetFontInfo(&info);

//   SetColor(BLACK);
//   MoveTo(el->bounds.left, el->bounds.top + info.ascent);
//   Print(hpEl->asStatElement.title);

//   char str[16] = {0};
//   char *s = NumToString(hpEl->asStatElement.stat->value, str);
//   s[0] = '/';
//   NumToString(hpEl->max->value + hpEl->asStatElement.tmp->value, s+1);
//   MoveTo(el->bounds.right - TextWidth(str), el->bounds.top + info.ascent);
//   Print(str);
// }

// static void HPElementInput(View *el, u16 input)
// {
//   MaxStatElement *statEl = (MaxStatElement*)el;
//   if (KeyPressed(BTN_SELECT)) {
//     Rect bounds = el->bounds;
//     bounds.left  = bounds.right - TextWidth("00/00");
//     bounds.right = bounds.left + TextWidth("00");
//     i32 newValue = EditNum(statEl->asStatElement.stat->value, &bounds, false, true);
//     if (UpdateStat(statEl->asStatElement.stat, newValue)) {
//       ClearScreen();
//       RedrawPage();
//       return;
//     }

//     if (KeyPressed(BTN_SELECT)) {
//       bounds = el->bounds;
//       bounds.left = bounds.right - TextWidth("00");
//       newValue = EditNum(statEl->asStatElement.tmp->value, &bounds, true, true);
//       FillRect(&bounds, BG);
//       if (UpdateStat(statEl->asStatElement.tmp, newValue)) {
//         ClearScreen();
//         RedrawPage();
//         return;
//       }
//     }
//     DrawHPElement(el);
//   }
// }

// PageElement *NewHPElement(Rect *bounds, char *title)
// {
//   MaxStatElement *hpEl = Alloc(sizeof(MaxStatElement));
//   InitStatElement(&hpEl->asStatElement, bounds, title, "HP", "TmpMaxHP");
//   hpEl->asView.draw = DrawHPElement;
//   hpEl->asView.onInput = HPElementInput;
//   hpEl->max = GetStat("MaxHP");
//   Assert(hpEl->max);
//   return (PageElement*)hpEl;
// }

// static void DrawStatElement(View *el)
// {
//   StatElement *statEl = (StatElement*)el;
//   FontInfo info;
//   GetFontInfo(&info);

//   SetColor(BLACK);
//   MoveTo(el->bounds.left, el->bounds.top + info.ascent);
//   Print(statEl->title);

//   i32 tmp = statEl->tmp ? statEl->tmp->value : 0;
//   char str[8] = {0};
//   NumToString(statEl->stat->value + tmp, str);
//   MoveTo(el->bounds.right - TextWidth(str), el->bounds.top + info.ascent);
//   Print(str);
// }

// static void StatElementInput(View *el, u16 input)
// {
//   StatElement *statEl = (StatElement*)el;
//   if (KeyPressed(BTN_SELECT)) {
//     if (statEl->edit(statEl)) {
//       ClearScreen();
//       RedrawPage();
//     }
//   } else if (KeyPressed(BTN_A) && statEl->action) {
//     statEl->action(statEl);
//   }
// }

// static bool StatElementEdit(StatElement *el)
// {
//   Rect bounds = el->asView.bounds;
//   bounds.left = bounds.right - TextWidth("00");
//   Stat *stat = el->tmp ? el->tmp : el->stat;
//   i32 newValue = EditNum(stat->value, &bounds, el->tmp ? true : false, true);
//   return UpdateStat(stat, newValue);
// }

// static void StatCheck(StatElement *el)
// {
//   DiceCheck(d20, 0);
// }

// static void ModCheck(StatElement *el)
// {
//   DiceCheck(d20, el->stat->value);
// }

// void InitStatElement(StatElement *el, Rect *bounds, char *title, char *statName, char *tmpName)
// {
//   InitPageElement(&el->asElement, bounds, DrawStatElement, StatElementInput);
//   el->title = title;
//   el->edit = StatElementEdit;
//   el->action = 0;
//   el->stat = GetStat(statName);
//   Assert(el->stat);
//   if (tmpName) {
//     el->tmp = GetStat(tmpName);
//     Assert(el->tmp);
//   } else {
//     el->tmp = 0;
//   }
// }

// PageElement *NewStatElement(Rect *bounds, char *title, char *statName, char *tmpName)
// {
//   StatElement *el = Alloc(sizeof(StatElement));
//   InitStatElement(el, bounds, title, statName, tmpName);
//   return (PageElement*)el;
// }

// static void DrawAbilityElement(View *el)
// {
//   StatElement *statEl = (StatElement*)el;
//   FontInfo info;
//   GetFontInfo(&info);

//   SetColor(BLACK);
//   MoveTo(el->bounds.left, el->bounds.top + info.ascent);
//   Print(statEl->title);

//   char str[8] = {0};
//   NumToString(statEl->stat->value, str);
//   MoveTo(el->bounds.right - TextWidth(str) - TextWidth("00") - 8, el->bounds.top + info.ascent);
//   Print(str);

//   ClearStr(str);
//   if (statEl->tmp->value >= 0) {
//     str[0] = '+';
//     NumToString(statEl->tmp->value, str+1);
//   } else {
//     NumToString(statEl->tmp->value, str);
//   }
//   MoveTo(el->bounds.right - TextWidth(str), el->bounds.top + info.ascent);
//   Print(str);
// }

// static bool AbilityElementEdit(StatElement *el)
// {
//   Rect bounds = el->asView.bounds;
//   bounds.left = bounds.right - 2*TextWidth("00") - 8;
//   bounds.right = bounds.left + TextWidth("00");
//   i32 newValue = EditNum(el->stat->value, &bounds, false, true);
//   return UpdateStat(el->stat, newValue);
// }

// PageElement *NewAbilityStatElement(Rect *bounds, char *title, char *statName, char *modName)
// {
//   StatElement *el = (StatElement*)NewStatElement(bounds, title, statName, modName);
//   el->asView.draw = DrawAbilityElement;
//   el->edit = AbilityElementEdit;
//   el->action = StatCheck;
//   return (PageElement*)el;
// }

// static void DrawModElement(View *el)
// {
//   StatElement *statEl = (StatElement*)el;
//   FontInfo info;
//   GetFontInfo(&info);

//   SetColor(BLACK);
//   MoveTo(el->bounds.left, el->bounds.top + info.ascent);
//   Print(statEl->title);

//   i32 tmp = statEl->tmp ? statEl->tmp->value : 0;
//   char str[8] = {0};
//   if (statEl->stat->value + tmp >= 0) {
//     str[0] = '+';
//     NumToString(statEl->stat->value + tmp, str+1);
//   } else {
//     NumToString(statEl->stat->value + tmp, str);
//   }
//   MoveTo(el->bounds.right - TextWidth(str), el->bounds.top + info.ascent);
//   Print(str);
// }

// static bool ModElementEdit(StatElement *el)
// {
//   Rect bounds = el->asView.bounds;
//   bounds.left = bounds.right - TextWidth("00");
//   Stat *stat = el->tmp ? el->tmp : el->stat;
//   i32 newValue = EditNum(stat->value, &bounds, true, true);
//   return UpdateStat(stat, newValue);
// }

// PageElement *NewModElement(Rect *bounds, char *title, char *statName, char *tmpName)
// {
//   StatElement *el = (StatElement*)NewStatElement(bounds, title, statName, tmpName);
//   el->asView.draw = DrawModElement;
//   el->edit = ModElementEdit;
//   el->action = ModCheck;
//   return (PageElement*)el;
// }

// static void DrawChargeElement(View *el)
// {
//   MaxStatElement *chEl = (MaxStatElement*)el;
//   FontInfo info;
//   GetFontInfo(&info);

//   SetColor(BLACK);
//   MoveTo(el->bounds.left, el->bounds.top + info.ascent);
//   Print(chEl->asStatElement.title);

//   Rect bounds = el->bounds;
//   i32 max = chEl->max->value;
//   bounds.left = bounds.right - 9*max - 1;
//   bounds.bottom = bounds.top + info.ascent + info.descent;
//   bounds.top = bounds.bottom - 10;
//   DrawCharges(chEl->asStatElement.stat->value, max, &bounds);
// }

// static bool ChargeElementEdit(StatElement *el)
// {
//   FontInfo info;
//   GetFontInfo(&info);
//   MaxStatElement *chEl = (MaxStatElement*)el;
//   Rect bounds = el->asView.bounds;
//   i32 max = chEl->max->value;
//   bounds.left = bounds.right - 9*max - 1;
//   bounds.bottom = bounds.top + info.ascent + info.descent;
//   bounds.top = bounds.bottom - 10;
//   i32 newValue = EditCharges(chEl->asStatElement.stat->value, max, &bounds);
//   return UpdateStat(chEl->asStatElement.stat, newValue);
// }

// PageElement *NewChargeElement(Rect *bounds, char *title, char *statName, char *maxStatName)
// {
//   MaxStatElement *hpEl = Alloc(sizeof(MaxStatElement));
//   InitStatElement(&hpEl->asStatElement, bounds, title, statName, 0);
//   hpEl->asView.draw = DrawChargeElement;
//   hpEl->asStatElement.edit = ChargeElementEdit;
//   hpEl->asStatElement.action = 0;
//   hpEl->max = GetStat(maxStatName);
//   Assert(hpEl->max);
//   return (PageElement*)hpEl;
// }

// void DrawTextElement(View *view)
// {
//   TextElement *el = (TextElement*)view;
//   FontInfo info;
//   GetFontInfo(&info);
//   char *cur = el->text;
//   i16 x = view->bounds.left;
//   i16 y = view->bounds.top + info.ascent;
//   MoveTo(x, y);
//   while (*cur) {
//     i16 w = CharWidth(*cur);
//     if (*cur == '\n' || x + w > view->bounds.right) {
//       x = view->bounds.left;
//       y += info.ascent + info.descent;
//       if (y + info.descent >= view->bounds.bottom) break;
//       MoveTo(x, y);
//     }
//     if (*cur == '\n') {
//       cur++;
//       continue;
//     }
//     x += w;
//     cur++;
//   }
// }

// void TextElementInput(View *view, u16 input)
// {

// }

// PageElement *NewTextElement(Rect *bounds, char *text)
// {
//   TextElement *textEl = Alloc(sizeof(TextElement));
//   InitPageElement(&textEl->asElement, bounds, DrawTextElement, TextElementInput);
//   textEl->text = text;
//   return (PageElement*)textEl;
// }
