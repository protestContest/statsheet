#include "views/dice_check_view.h"
#include "kit/canvas.h"
#include "kit/input.h"
#include "kit/interrupt.h"
#include "kit/mem.h"
#include "kit/text.h"
#include "kit/str.h"
#include "ui.h"
#include "views/page_list.h"

static void DrawResult(DiceCheckView *dcView)
{
  FontInfo info;
  GetFontInfo(&info);
  i16 cx = (dcView->asView.bounds.right+dcView->asView.bounds.left)/2;
  i16 cy = (dcView->asView.bounds.bottom+dcView->asView.bounds.top)/2;

  Rect resultRect = {cx, dcView->asView.bounds.top + 3, dcView->asView.bounds.right-3, dcView->btn.asView.bounds.top};
  EraseRect(&resultRect);

  char *natLabel = dcView->result == 20 ? "NAT 20" : dcView->result == 1 ? "NAT 1" : 0;
  if (natLabel) {
    i16 w = TextWidth(natLabel) + 8;
    Rect natRect = {cx + 40 - w/2, cy - 20 - info.ascent - 2, cx + 40 + w/2, cy - 20 + info.descent + 2};
    SetColor(BLACK);
    FrameRect(&natRect);
    MoveTo(natRect.left + 5, natRect.top + info.ascent + 2);
    Print(natLabel);
  }

  i32 mod = dcView->modCtl.value;
  char str[16] = {0};
  CopyStr("Result: ^", str);
  FormatInt(str, dcView->result + mod, 16);
  MoveTo(cx + 40 - TextWidth(str)/2, cy);
  SetColor(BLACK);
  Print(str);
}

static void DrawDiceCheck(View *view)
{
  DiceCheckView *dcView = (DiceCheckView*)view;
  DrawWindow(&view->bounds);
  DrawView(&dcView->dice.asView);
  DrawView(&dcView->modCtl.asView);
  DrawView(&dcView->btn.asView);
  ShowArrows(&dcView->selected->bounds, dirV);

  SetColor(BLACK);
  Rect numBounds = dcView->modCtl.asView.bounds;
  InsetRect(&numBounds, -1, -1);
  FrameRect(&numBounds);

  if (dcView->rolled) {
    DrawResult(dcView);
  }
}

static bool InputDiceCheck(View *view, u16 input)
{
  DiceCheckView *dcView = (DiceCheckView*)view;
  InputView(&dcView->btn.asView, input);

  if (KeyPressed(BTN_LEFT) || KeyPressed(BTN_RIGHT)) {
    if (dcView->selected == &dcView->dice.asView) {
      dcView->selected = &dcView->modCtl.asView;
    } else {
      dcView->selected = &dcView->dice.asView;
    }
    ShowArrows(&dcView->selected->bounds, dirV);
  } else if (KeyPressed(BTN_B)) {
    return true;
  } else if (KeyReleased(BTN_A)) {
    dcView->rolled = true;
    dcView->result = RollDice(dcView->dice.curDie);
    DrawResult(dcView);
  } else {
    InputView(dcView->selected, input);
  }

  return false;
}

static void DiceCheckViewActivate(View *view, bool active)
{
  if (active) {
    do {
      VSync();
    } while (GetInput() & BTN_A);
    while (KeyReleased(BTN_A)) {
      VSync();
      GetInput();
    }
  } else {
    SetBackground(BG);
    HideAllObjs();
    RedrawPage();
  }
}

void InitDiceCheckView(DiceCheckView *view, Rect *bounds, u32 die, i32 mod)
{
  i16 lineHeight = LineHeight();
  InitView(&view->asView, bounds, DrawDiceCheck, InputDiceCheck, DiceCheckViewActivate);
  view->selected = &view->dice.asView;
  view->rolled = false;
  view->result = 0;

  i16 cx = (bounds->right+bounds->left)/2;
  i16 cy = (bounds->bottom+bounds->top)/2;

  Rect diceBounds = {cx-24-30, cy-24, cx+24-30, cy+24};
  InitDiceView(&view->dice, &diceBounds, die, dirV);

  i16 numWidth = TextWidth("000");
  Rect modBounds = {cx-75-numWidth/2, cy-lineHeight/2, cx-75+numWidth/2, cy+lineHeight/2};
  InitNumControl(&view->modCtl, &modBounds, mod, dirV, true);

  Rect btnBounds = {cx+20, cy + LineHeight(), cx+60, cy + LineHeight() + 16};
  InitButton(&view->btn, &btnBounds, "Roll");
}
