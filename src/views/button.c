#include "views/button.h"
#include "kit/input.h"
#include "kit/text.h"

static void DrawButtonUp(Rect *bounds)
{
  Rect r = *bounds;
  SetColor(RGB(48, 48, 48));
  MoveTo(r.left, r.bottom-3);
  LineTo(r.left, r.top+2);
  LineTo(r.left+2, r.top);
  LineTo(r.right-3, r.top);
  LineTo(r.right-1, r.top+2);
  LineTo(r.right-1, r.bottom-3);
  SetColor(BLACK);
  LineTo(r.right-3, r.bottom-1);
  LineTo(r.left+2, r.bottom-1);
  InsetRect(&r, 1, 1);
  SetColor(RGB(128, 128, 128));
  MoveTo(r.left, r.bottom-2);
  LineTo(r.left, r.top+1);
  LineTo(r.left+1, r.top);
  LineTo(r.right-2, r.top);
  SetColor(RGB(48, 48, 48));
  LineTo(r.right-1, r.top+1);
  LineTo(r.right-1, r.bottom-2);
  LineTo(r.right-2, r.bottom-1);
  LineTo(r.left, r.bottom-1);
  InsetRect(&r, 1, 1);
  FillRect(&r, RGB(90, 90, 90));
}

static void DrawButtonDown(Rect *bounds)
{
  Rect r = *bounds;
  InsetRect(&r, 2, 2);
  FillRect(&r, RGB(90,90,90));
  InsetRect(&r, -2, -2);

  SetColor(BLACK);
  MoveTo(r.left, r.bottom-3);
  LineTo(r.left, r.top+2);
  LineTo(r.left+2, r.top);
  LineTo(r.right-3, r.top);

  SetColor(RGB(48,48,48));
  MoveTo(r.left+1, r.bottom-2);
  LineTo(r.left+1, r.top+2);
  LineTo(r.left+2, r.top+2);
  LineTo(r.left+2, r.top+1);
  LineTo(r.right-2, r.top+1);
  LineTo(r.right-1, r.top+2);
  LineTo(r.right-1, r.bottom-3);
  LineTo(r.right-3, r.bottom-1);
  LineTo(r.left+2, r.bottom-1);
  LineTo(r.left+2, r.bottom-2);

  SetColor(RGB(128,128,128));
  MoveTo(r.right-2, r.top+2);
  LineTo(r.right-2, r.bottom-3);
  LineTo(r.right-3, r.bottom-2);
  LineTo(r.left+3, r.bottom-2);
}

static void DrawButton(View *el)
{
  Button *button = (Button*)el;
  if (button->active) {
    DrawButtonDown(&el->bounds);
  } else {
    DrawButtonUp(&el->bounds);
  }

  FontInfo info;
  GetFontInfo(&info);
  MoveTo((el->bounds.right+el->bounds.left)/2 - TextWidth(button->title)/2,
         (el->bounds.bottom+el->bounds.top)/2 - (info.ascent+info.descent)/2 + info.ascent);
  SetColor(WHITE);
  Print(button->title);
}

static bool ButtonInput(View *el, u16 input)
{
  Button *button = (Button*)el;
  button->active = (input & BTN_A) ? true : false;

  if (KeyPressed(BTN_A) || KeyReleased(BTN_A)) {
    EraseRect(&el->bounds);
    DrawButton(el);
  }

  return false;
}

void InitButton(Button *button, Rect *bounds, char *title)
{
  InitView(&button->asView, bounds, DrawButton, ButtonInput, 0, 0);
  button->title = title;
  button->active = false;
}