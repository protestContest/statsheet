#include "views/dice_view.h"
#include "kit/canvas.h"
#include "kit/input.h"
#include "kit/sprite.h"
#include "kit/res.h"
#include "kit/math.h"
#include "kit/time.h"

#define diceObj   5
#define numDice   6

static char *diceImgs[] = {
  "d4.tga",
  "d6.tga",
  "d8.tga",
  "d10.tga",
  "d12.tga",
  "d20.tga"
};

static SpriteFrame diceFrames[] = {
  {612, 0, 0},
  {676, 0, 0},
  {740, 0, 0},
  {804, 0, 0}
};
static AnimatedSprite diceSprite;

static void LoadDice(u32 index)
{
  TGA *tiles = ResData(GetResource(diceImgs[index]));

  SetPalette(1, (u8*)tiles->data, tiles->paletteSize, tiles->paletteDepth);
  SetObjPalette(diceObj, 1);

  Rect r = {0, 0, 64, 48};
  LoadTiles(tiles, &r, diceSprite.frames[0].baseTile);
  OffsetRect(&r, 64, 0);
  LoadTiles(tiles, &r, diceSprite.frames[1].baseTile);
  OffsetRect(&r, 64, 0);
  LoadTiles(tiles, &r, diceSprite.frames[2].baseTile);
  OffsetRect(&r, 64, 0);
  LoadTiles(tiles, &r, diceSprite.frames[3].baseTile);
  AssignSprite(diceObj, &diceSprite);
}

static void DrawDiceView(View *view)
{
  PlaceObj(diceObj, view->bounds.left, view->bounds.top);
  ShowObj(diceObj);
}

static bool DiceViewInput(View *view, u16 input)
{
  DiceView *diceEl = (DiceView*)view;

  if (diceEl->selectDir) {
    if (KeyPressed(BTN_LEFT)) {
      diceEl->curDie = (diceEl->curDie > 0) ? diceEl->curDie-1 : numDice-1;
    } else if (KeyPressed(BTN_RIGHT)) {
      diceEl->curDie = (diceEl->curDie + 1) % numDice;
    }
    if (input & (BTN_LEFT | BTN_RIGHT)) {
      LoadDice(diceEl->curDie);
    }
  } else {
    if (KeyPressed(BTN_UP)) {
      diceEl->curDie = (diceEl->curDie > 0) ? diceEl->curDie-1 : numDice-1;
    } else if (KeyPressed(BTN_DOWN)) {
      diceEl->curDie = (diceEl->curDie + 1) % numDice;
    }
    if (input & (BTN_UP | BTN_DOWN)) {
      LoadDice(diceEl->curDie);
    }
  }
  return false;
}

void InitDiceView(DiceView *view, Rect *bounds, u32 die, VHSelect selectDir)
{
  InitSprite(&diceSprite,    Obj64x64, 3, ArrayCount(diceFrames), diceFrames);

  InitView(&view->asView, bounds, DrawDiceView, DiceViewInput, 0);
  view->curDie = die;
  view->selectDir = selectDir;
  HideObj(diceObj);
  LoadDice(view->curDie);
}

static u32 diceSides[] = {4,6,8,10,12,20};
u32 RollDice(u32 die)
{
  SeedRandom(TickCount());
  u32 end = TickCount() + 30;
  while (TickCount() < end) {
    UpdateSprite(diceObj);
  }
  SetSpriteFrame(diceObj, 0);

  return RandomBetween(0, diceSides[die]) + 1;
}
