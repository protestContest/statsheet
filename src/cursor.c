#include "cursor.h"
#include "kit.h"

#define cursorObj 0

void InitCursor(void)
{
  SpriteFrame cursorFrame = {512, 0, 0};
  AnimatedSprite cursor;
  InitSprite(&cursor, Obj8x8, 0, 1, &cursorFrame);
  SetObjDisplay(cursorObj, HideObj);
  AssignSprite(cursorObj, &cursor);
}

void ShowCursor(void)
{
  SetObjDisplay(cursorObj, ShowObj);
}

void HideCursor(void)
{
  SetObjDisplay(cursorObj, HideObj);
}

void PlaceCursor(i16 x, i16 y)
{
  PlaceObj(cursorObj, x-8, y-4);
}
