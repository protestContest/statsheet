#include "ui.h"
#include "kit/canvas.h"
#include "kit/input.h"
#include "kit/sprite.h"
#include "kit/text.h"

#define cursorObj 0

#define arrowTile     512
#define arrowVertTile 513

void InitUI(void)
{
  SetBackground(BG);
  ClearScreen();

  InitObj(arrowLeftObj);
  HideObj(arrowLeftObj);
  SetObjSize(arrowLeftObj, Obj8x8);
  SetObjSprite(arrowLeftObj, arrowTile);
  SetObjFlipH(arrowLeftObj, true);

  InitObj(arrowRightObj);
  HideObj(arrowRightObj);
  SetObjSize(arrowRightObj, Obj8x8);
  SetObjSprite(arrowRightObj, arrowTile);

  InitObj(arrowUpObj);
  HideObj(arrowUpObj);
  SetObjSize(arrowUpObj, Obj8x8);
  SetObjSprite(arrowUpObj, arrowVertTile);
  SetObjFlipV(arrowUpObj, true);

  InitObj(arrowDownObj);
  HideObj(arrowDownObj);
  SetObjSize(arrowDownObj, Obj8x8);
  SetObjSprite(arrowDownObj, arrowVertTile);

  InitObj(cursorObj);
  HideObj(cursorObj);
  SetObjSize(cursorObj, Obj8x8);
  SetObjSprite(cursorObj, arrowTile);
}

void DrawWindow(Rect *bounds)
{
  Rect r = *bounds;
  SetColor(RGB(48,48,48));
  FrameRect(&r);
  InsetRect(&r, 1, 1);
  FrameRect(&r);
  InsetRect(&r, 1, 1);
  SetColor(RGB(90,90,90));
  FrameRect(&r);
  InsetRect(&r, 1, 1);
  SetBackground(WIN_BG);
  EraseRect(&r);
}

void Alert(char *text)
{
  FontInfo info;
  GetFontInfo(&info);
  i16 width = TextWidth(text) + 16;
  i16 height = TextHeight(text) + 16;

  Rect bounds;
  bounds.left = SCREEN_W/2 - width/2;
  bounds.top = SCREEN_H/2 - height/2;
  bounds.right = bounds.left+width;
  bounds.bottom = bounds.top+height;

  DrawWindow(&bounds);
  SetColor(BLACK);
  MoveTo(bounds.left + 9, bounds.top + 9 + info.ascent);
  Print(text);
  SetColor(WHITE);
  MoveTo(bounds.left + 8, bounds.top + 8 + info.ascent);
  Print(text);

  WaitForInput();
  EraseRect(&bounds);
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

void ShowArrows(Rect *bounds, VHSelect selectDir)
{
  if (selectDir == dirH) {
    HideObj(arrowUpObj);
    HideObj(arrowDownObj);
    PlaceObj(arrowLeftObj, bounds->left - 7, bounds->top + (bounds->bottom - bounds->top)/2 - 3);
    ShowObj(arrowLeftObj);
    PlaceObj(arrowRightObj, bounds->right - 1, bounds->top + (bounds->bottom - bounds->top)/2 - 3);
    ShowObj(arrowRightObj);
  } else {
    HideObj(arrowLeftObj);
    HideObj(arrowRightObj);
    PlaceObj(arrowUpObj, bounds->left + (bounds->right - bounds->left)/2 - 4, bounds->top - 6);
    ShowObj(arrowUpObj);
    PlaceObj(arrowDownObj, bounds->left + (bounds->right - bounds->left)/2 - 4, bounds->bottom);
    ShowObj(arrowDownObj);
  }
}

void ShowUpArrow(i16 x, i16 y)
{
  PlaceObj(arrowUpObj, x-4, y);
  ShowObj(arrowUpObj);
}

void ShowDownArrow(i16 x, i16 y)
{
  PlaceObj(arrowDownObj, x-4, y);
  ShowObj(arrowDownObj);
}

void HideArrow(Direction arrow)
{
  switch (arrow) {
  case dirUp:
    HideObj(arrowUpObj);
    break;
  case dirDown:
    HideObj(arrowDownObj);
    break;
  case dirLeft:
    HideObj(arrowLeftObj);
    break;
  case dirRight:
    HideObj(arrowRightObj);
    break;
  }
}

void HideArrows(void)
{
  HideObj(arrowUpObj);
  HideObj(arrowDownObj);
  HideObj(arrowLeftObj);
  HideObj(arrowRightObj);
}

void HideAllObjs(void)
{
  for (u32 i = 0; i < 128; i++) {
    HideObj(i);
  }
}
