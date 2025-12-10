#include "ui.h"

#define cursorObj 0

#define arrowTile 512

void InitUI(void)
{
  InitObj(arrowLeftObj);
  HideObj(arrowLeftObj);
  SetObjSize(arrowLeftObj, Obj8x8);
  SetObjSprite(arrowLeftObj, arrowTile);
  SetObjFlipH(arrowLeftObj, true);

  InitObj(arrowRightObj);
  HideObj(arrowRightObj);
  SetObjSize(arrowRightObj, Obj8x8);
  SetObjSprite(arrowRightObj, arrowTile);

  InitObj(cursorObj);
  HideObj(cursorObj);
  SetObjSize(cursorObj, Obj8x8);
  SetObjSprite(cursorObj, arrowTile);
}

void ShowWindow(Rect *bounds)
{
  i16 x = bounds->left;
  i16 y = bounds->top;
  i16 width = bounds->right - bounds->left;
  i16 height = bounds->bottom - bounds->top;
  Rect bg = {x+7, y+7, x+width-7, y+height-7};
  FillRect(&bg, RGB(0, 96, 136));

  SetColor(BLACK);
  MoveTo(x, y+1);
  LineTo(x+1, y);
  MoveTo(x+width-2, y);
  LineTo(x+width-1, y+1);
  LineTo(x+width-1, y+height-2);
  LineTo(x+width-2, y+height-1);
  LineTo(x+1, y+height-1);
  LineTo(x, y+height-2);

  SetColor(RGB(80, 80, 80));
  MoveTo(x+width-2, y+height-2);
  LineTo(x+width-2, y+height-2);

  MoveTo(x+2, y);
  LineTo(x+width-3, y);
  LineTo(x+width-2, y+1);
  SetColor(RGB(160, 160, 160));
  LineTo(x+width-2, y+height-3);
  LineTo(x+width-3, y+height-3);
  LineTo(x+width-3, y+height-2);
  LineTo(x+2, y+height-2);
  LineTo(x+1, y+height-3);
  MoveTo(x+1, y+height-2);
  SetColor(RGB(80,80,80));
  LineTo(x, y+height-3);
  LineTo(x, y+2);
  LineTo(x+2, y);

  SetColor(RGB(248,248,248));
  MoveTo(x+2, y+1);
  LineTo(x+width-3, y+1);
  LineTo(x+width-3, y+height-4);
  MoveTo(x+width-4, y+height-3);
  LineTo(x+2, y+height-3);
  MoveTo(x+1, y+height-4);
  LineTo(x+1, y+2);

  SetColor(RGB(160, 160, 160));
  MoveTo(x+2,y+height-4);
  LineTo(x+2, y+2);
  LineTo(x+width-4, y+2);
  SetColor(RGB(80, 80, 80));
  MoveTo(x+width-4, y+3);
  LineTo(x+width-4, y+height-4);
  MoveTo(x+width-5, y+height-5);
  LineTo(x+width-5, y+height-4);
  LineTo(x+3, y+height-4);
  MoveTo(x+3, y+3);
  LineTo(x+3, y+3);

  SetColor(BLACK);
  MoveTo(x+4, y+height-5);
  LineTo(x+3, y+height-5);
  LineTo(x+3, y+4);
  LineTo(x+4, y+4);
  LineTo(x+4, y+3);
  LineTo(x+width-5, y+3);
  LineTo(x+width-5, y+4);

  SetColor(RGB(0,72,80));
  MoveTo(x+4, y+height-6);
  LineTo(x+4, y+5);
  MoveTo(x+5, y+4);
  LineTo(x+width-6, y+4);
  SetColor(RGB(0,80,96));
  MoveTo(x+5, y+height-6);
  LineTo(x+5, y+5);
  LineTo(x+width-6, y+5);
  SetColor(RGB(0, 88, 112));
  MoveTo(x+6, y+height-7);
  LineTo(x+6, y+6);
  LineTo(x+width-7, y+6);

  SetColor(RGB(8,128,184));
  MoveTo(x+width-5, y+5);
  LineTo(x+width-5, y+height-6);
  MoveTo(x+width-6, y+height-5);
  LineTo(x+5, y+height-5);
  SetColor(RGB(8, 112, 168));
  MoveTo(x+width-6, y+6);
  LineTo(x+width-6, y+height-6);
  LineTo(x+6, y+height-6);
  SetColor(RGB(8, 104, 152));
  MoveTo(x+width-7, y+7);
  LineTo(x+width-7, y+height-7);
  LineTo(x+7, y+height-7);
}

void TextWindow(char *text, Rect *bounds)
{
  FontInfo finfo;
  GetFontInfo(&finfo);

  ShowWindow(bounds);
  SetColor(BLACK);
  MoveTo(bounds->left + 9, bounds->top + 9 + finfo.ascent);
  Print(text);
  SetColor(WHITE);
  MoveTo(bounds->left + 8, bounds->top + 8 + finfo.ascent);
  Print(text);
}

void Alert(char *text)
{
  i16 width = TextWidth(text) + 16;
  i16 height = TextHeight(text) + 16;

  Rect bounds;
  bounds.left = SCREEN_W/2 - width/2;
  bounds.top = SCREEN_H/2 - height/2;
  bounds.right = bounds.left+width;
  bounds.bottom = bounds.top+height;

  TextWindow(text, &bounds);
  WaitForInput();
  FillRect(&bounds, BG);
}

void ShowCursor(void)
{
  ShowObj(cursorObj);
}

void HideCursor(void)
{
  HideObj(cursorObj);
}

void PlaceCursor(i16 x, i16 y)
{
  PlaceObj(cursorObj, x-8, y-4);
}

void ShowArrows(Rect *bounds)
{
  PlaceObj(arrowLeftObj, bounds->left - 7, bounds->top + (bounds->bottom - bounds->top)/2 - 4);
  ShowObj(arrowLeftObj);
  PlaceObj(arrowRightObj, bounds->right - 1, bounds->top + (bounds->bottom - bounds->top)/2 - 4);
  ShowObj(arrowRightObj);
}

void HideArrows(void)
{
  HideObj(arrowLeftObj);
  HideObj(arrowRightObj);
}

void DrawNum(i32 num, i16 x, i16 y, bool showSign)
{
  char str[8] = {0};
  if (showSign && num >= 0) {
    str[0] = '+';
    NumToString(num, str+1);
  } else {
    NumToString(num, str);
  }
  MoveTo(x - TextWidth(str), y);
  Print(str);
}

void DrawCharges(i32 used, i32 max, Rect *bounds)
{
  Rect r = *bounds;
  r.bottom = r.top + 10;
  SetColor(BLACK);
  FrameRect(&r);
  for (i32 i = 1; i < max; i++) {
    MoveTo(r.left + 9*i, r.top);
    Line(0, 9);
  }
  r.left += 2;
  r.top += 2;
  r.bottom = r.top + 6;
  r.right = r.left + 6;
  for (i32 i = 0; i < used; i++) {
    FillRect(&r, BLACK);
    OffsetRect(&r, 9, 0);
  }
}

i32 EditNum(i32 num, Rect *bounds, bool showSign)
{
  i32 original = num;
  FontInfo info;
  GetFontInfo(&info);
  SetColor(BLACK);
  InsetRect(bounds, -2, -2);
  FrameRect(bounds);
  InsetRect(bounds, 1, 1);

  i16 x = bounds->right-1;
  i16 y = bounds->top+1+info.ascent;

  FillRect(bounds, BG);
  DrawNum(num, x, y, showSign);

  ShowArrows(bounds);

  while (true) {
    VSync();
    u16 input = GetInput();
    if (KeyPressed(BTN_A)) {
      break;
    } else if (KeyPressed(BTN_B) || KeyPressed(BTN_SELECT)) {
      num = original;
      break;
    } else if (KeyPressed(BTN_LEFT)) {
      num--;
    } else if (KeyPressed(BTN_RIGHT)) {
      num++;
    }
    if (input & (BTN_LEFT | BTN_RIGHT)) {
      FillRect(bounds, BG);
      DrawNum(num, x, y, showSign);
    }
  }

  HideArrows();
  InsetRect(bounds, -1, -1);
  FillRect(bounds, BG);
  DrawNum(num, x, y, showSign);
  return num;
}

i32 EditCharges(i32 used, i32 max, Rect *bounds)
{
  i32 original = used;
  FontInfo info;
  GetFontInfo(&info);
  SetColor(BLACK);
  InsetRect(bounds, -2, -2);
  FrameRect(bounds);
  InsetRect(bounds, 2, 2);

  PlaceObj(arrowLeftObj, bounds->left - 7, bounds->top + 3);
  ShowObj(arrowLeftObj);
  PlaceObj(arrowRightObj, bounds->right - 1, bounds->top + 3);
  ShowObj(arrowRightObj);

  while (true) {
    VSync();
    u16 input = GetInput();
    if (KeyPressed(BTN_A)) {
      break;
    } else if (KeyPressed(BTN_B) || KeyPressed(BTN_SELECT)) {
      used = original;
      break;
    } else if (KeyPressed(BTN_LEFT) && used > 0) {
      used--;
    } else if (KeyPressed(BTN_RIGHT) && used < max) {
      used++;
    }
    if (input & (BTN_LEFT | BTN_RIGHT)) {
      FillRect(bounds, BG);
      DrawCharges(used, max, bounds);
    }
  }

  HideObj(arrowLeftObj);
  HideObj(arrowRightObj);

  InsetRect(bounds, -2, -2);
  FillRect(bounds, BG);
  InsetRect(bounds, 2, 2);
  DrawCharges(used, max, bounds);

  return used;
}

void HideAllObjs(void)
{
  for (u32 i = 0; i < 128; i++) {
    HideObj(i);
  }
}
