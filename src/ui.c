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
  PlaceObj(cursorObj, x-8, y-3);
}

void ShowArrows(Rect *bounds, bool selectDir)
{
  if (selectDir == horizontal) {
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



// void DrawCharges(i32 used, i32 max, Rect *bounds)
// {
//   Rect r = *bounds;
//   r.bottom = r.top + 10;
//   SetColor(BLACK);
//   FrameRect(&r);
//   for (i32 i = 1; i < max; i++) {
//     MoveTo(r.left + 9*i, r.top);
//     Line(0, 9);
//   }
//   r.left += 2;
//   r.top += 2;
//   r.bottom = r.top + 6;
//   r.right = r.left + 6;
//   for (i32 i = 0; i < used; i++) {
//     FillRect(&r, BLACK);
//     OffsetRect(&r, 9, 0);
//   }
// }

// i32 NumInput(i32 num, u16 input, Rect *bounds, bool showSign, bool selectDir)
// {
//   FontInfo info;
//   GetFontInfo(&info);
//   i16 x = bounds->right-1;
//   i16 y = bounds->top+1+info.ascent;
//   if ((selectDir && KeyPressed(BTN_LEFT)) || (!selectDir && KeyPressed(BTN_DOWN))) {
//     num--;
//   } else if ((selectDir && KeyPressed(BTN_RIGHT)) || (!selectDir && KeyPressed(BTN_UP))) {
//     num++;
//   }
//   u16 changeMask = selectDir ? (BTN_LEFT|BTN_RIGHT) : (BTN_UP|BTN_DOWN);
//   if (input & changeMask) {
//     EraseRect(bounds);
//     DrawNum(num, x, y, showSign);
//   }
//   return num;
// }

// i32 EditNum(i32 num, Rect *bounds, bool showSign, bool selectDir)
// {
//   i32 original = num;
//   FontInfo info;
//   GetFontInfo(&info);
//   SetColor(BLACK);
//   InsetRect(bounds, -2, -2);
//   FrameRect(bounds);
//   InsetRect(bounds, 1, 1);

//   i16 x = bounds->right-1;
//   i16 y = bounds->top+1+info.ascent;

//   EraseRect(bounds);
//   DrawNum(num, x, y, showSign);

//   ShowArrows(bounds, selectDir);

//   while (true) {
//     VSync();
//     u16 input = GetInput();
//     if (KeyPressed(BTN_A)) {
//       break;
//     } else if (KeyPressed(BTN_B) || KeyPressed(BTN_SELECT)) {
//       num = original;
//       break;
//     } else {
//       num = NumInput(num, input, bounds, showSign, selectDir);
//     }
//   }

//   HideArrows();
//   InsetRect(bounds, -1, -1);
//   EraseRect(bounds);
//   DrawNum(num, x, y, showSign);
//   return num;
// }

// i32 EditCharges(i32 used, i32 max, Rect *bounds)
// {
//   i32 original = used;
//   FontInfo info;
//   GetFontInfo(&info);
//   SetColor(BLACK);
//   InsetRect(bounds, -1, -1);
//   ShowArrows(bounds, true);
//   InsetRect(bounds, -1, -1);
//   FrameRect(bounds);
//   InsetRect(bounds, 2, 2);


//   while (true) {
//     VSync();
//     u16 input = GetInput();
//     if (KeyPressed(BTN_A)) {
//       break;
//     } else if (KeyPressed(BTN_B) || KeyPressed(BTN_SELECT)) {
//       used = original;
//       break;
//     } else if (KeyPressed(BTN_LEFT) && used > 0) {
//       used--;
//     } else if (KeyPressed(BTN_RIGHT) && used < max) {
//       used++;
//     }
//     if (input & (BTN_LEFT | BTN_RIGHT)) {
//       EraseRect(bounds);
//       DrawCharges(used, max, bounds);
//     }
//   }

//   HideArrows();

//   InsetRect(bounds, -2, -2);
//   EraseRect(bounds);
//   InsetRect(bounds, 2, 2);
//   DrawCharges(used, max, bounds);

//   return used;
// }

