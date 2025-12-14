// #include "dice.h"
// #include "elements.h"
// #include "kit/canvas.h"
// #include "page.h"
// #include "ui.h"

// #define diceObj   5
// #define numDice   6
// static u32 diceSides[] = {4,6,8,10,12,20};
// static char *diceImgs[] = {
//   "d4.tga",
//   "d6.tga",
//   "d8.tga",
//   "d10.tga",
//   "d12.tga",
//   "d20.tga"
// };
// static void LoadDice(u32 index, AnimatedSprite *sprite)
// {
//   TGA *tiles = ResData(GetResource(diceImgs[index]));

//   SetPalette(1, (u8*)tiles->data, tiles->paletteSize, tiles->paletteDepth);
//   SetObjPalette(diceObj, 1);

//   Rect r = {0, 0, 64, 48};
//   LoadTiles(tiles, &r, sprite->frames[0].baseTile);
//   OffsetRect(&r, 64, 0);
//   LoadTiles(tiles, &r, sprite->frames[1].baseTile);
//   OffsetRect(&r, 64, 0);
//   LoadTiles(tiles, &r, sprite->frames[2].baseTile);
//   OffsetRect(&r, 64, 0);
//   LoadTiles(tiles, &r, sprite->frames[3].baseTile);
//   AssignSprite(diceObj, sprite);
// }

// static void DrawDiceElement(View *el)
// {
//   PlaceObj(diceObj, el->bounds.left, el->bounds.top);
//   ShowObj(diceObj);
// }

// static void DiceElementInput(View *el, u16 input)
// {
//   DiceElement *diceEl = (DiceElement*)el;

//   if (diceEl->selectDir) {
//     if (KeyPressed(BTN_LEFT)) {
//       diceEl->curDie = (diceEl->curDie > 0) ? diceEl->curDie-1 : numDice-1;
//     } else if (KeyPressed(BTN_RIGHT)) {
//       diceEl->curDie = (diceEl->curDie + 1) % numDice;
//     }
//     if (input & (BTN_LEFT | BTN_RIGHT)) {
//       LoadDice(diceEl->curDie, diceEl->sprite);
//     }
//   } else {
//     if (KeyPressed(BTN_UP)) {
//       diceEl->curDie = (diceEl->curDie > 0) ? diceEl->curDie-1 : numDice-1;
//     } else if (KeyPressed(BTN_DOWN)) {
//       diceEl->curDie = (diceEl->curDie + 1) % numDice;
//     }
//     if (input & (BTN_UP | BTN_DOWN)) {
//       LoadDice(diceEl->curDie, diceEl->sprite);
//     }
//   }
// }

// static SpriteFrame diceFrames[] = {
//   {612, 0, 0},
//   {676, 0, 0},
//   {740, 0, 0},
//   {804, 0, 0}
// };
// static AnimatedSprite diceSprite;

// PageElement *NewDiceElement(Rect *bounds, bool selectDir)
// {
//   InitSprite(&diceSprite,    Obj64x64, 3, ArrayCount(diceFrames), diceFrames);

//   DiceElement *el = Alloc(sizeof(DiceElement));
//   InitPageElement(&el->asElement, bounds, DrawDiceElement, DiceElementInput);
//   el->curDie = 5;
//   el->selectDir = selectDir;
//   el->sprite = &diceSprite;
//   HideObj(diceObj);
//   LoadDice(el->curDie, &diceSprite);
//   return (PageElement*)el;
// }

// u32 RollDice(DiceElement *el)
// {
//   SeedRandom(TickCount());
//   u32 end = TickCount() + 30;
//   while (TickCount() < end) {
//     UpdateSprite(diceObj);
//   }
//   SetSpriteFrame(diceObj, 0);

//   return RandomBetween(0, diceSides[el->curDie]) + 1;
// }

// void DiceCheck(u32 die, i32 mod)
// {
//   FontInfo info;
//   GetFontInfo(&info);
//   u32 lineHeight = info.ascent + info.descent;
//   i16 w = 200;
//   i16 h = 100;
//   Rect wBounds = {SCREEN_W/2-w/2, SCREEN_H/2-h/2, SCREEN_W/2+w/2, SCREEN_H/2+h/2};
//   Rect diceBounds = {SCREEN_W/2-24-30, SCREEN_H/2-24, SCREEN_W/2+24-30, SCREEN_H/2+24};
//   Rect btnBounds = {
//     SCREEN_W/2+20, SCREEN_H/2 + lineHeight,
//     SCREEN_W/2+60, SCREEN_H/2 + lineHeight + 16
//   };

//   i16 numWidth = TextWidth("000");
//   Rect numBounds = {SCREEN_W/2-75-numWidth/2, SCREEN_H/2-lineHeight/2, SCREEN_W/2-75+numWidth/2, SCREEN_H/2+lineHeight/2};

//   DiceElement *dice = (DiceElement*)NewDiceElement(&diceBounds, false);
//   PageElement *btn = NewButton(&btnBounds, "Roll");
//   bool diceSelected = true;

//   HideAllObjs();

//   ShowWindow(&wBounds);
//   dice->asView.draw(&dice->asView);
//   btn->asView.draw(&btn->asView);
//   ShowArrows(&dice->asView.bounds, dice->selectDir);

//   SetColor(BLACK);
//   InsetRect(&numBounds, -2, -2);
//   FrameRect(&numBounds);
//   InsetRect(&numBounds, 1, 1);

//   i16 x = numBounds.right-1;
//   i16 y = numBounds.top+1+info.ascent;
//   DrawNum(mod, x, y, true);

//   while (KeyPressed(BTN_A)) {
//     VSync();
//     GetInput();
//   }

//   while (true) {
//     VSync();
//     u16 input = GetInput();

//     btn->asView.onInput(&btn->asView, input);

//     if (KeyPressed(BTN_LEFT) || KeyPressed(BTN_RIGHT)) {
//       diceSelected = !diceSelected;
//       if (diceSelected) {
//         ShowArrows(&dice->asView.bounds, false);
//       } else {
//         ShowArrows(&numBounds, false);
//       }
//     } else if (KeyPressed(BTN_B)) {
//       break;
//     } else if (KeyReleased(BTN_A)) {
//       Rect r = wBounds;
//       InsetRect(&r, 3, 3);
//       r.left = SCREEN_W/2;
//       r.bottom = SCREEN_H/2 + lineHeight;
//       EraseRect(&r);

//       i32 value = RollDice(dice);
//       if (value == 20) {
//         i16 w = TextWidth("NAT 20");
//         Rect r = {
//           SCREEN_W/2 + 40 - w/2 - 4,
//           SCREEN_H/2 - 20 - info.ascent - 2,
//           SCREEN_W/2 + 40 + w/2 + 4,
//           SCREEN_H/2 - 20 + info.descent + 2
//         };
//         SetColor(BLACK);
//         FrameRect(&r);
//         MoveTo(r.left + 5, r.top + info.ascent + 2);
//         Print("NAT 20");
//       } else if (value == 1) {
//         i16 w = TextWidth("NAT 1");
//         Rect r = {
//           SCREEN_W/2 + 40 - w/2 - 4,
//           SCREEN_H/2 - 20 - info.ascent - 2,
//           SCREEN_W/2 + 40 + w/2 + 4,
//           SCREEN_H/2 - 20 + info.descent + 2
//         };
//         SetColor(BLACK);
//         FrameRect(&r);
//         MoveTo(r.left + 5, r.top + info.ascent + 2);
//         Print("NAT 1");
//       }

//       char str[16] = {0};
//       CopyStr("Result: ^", str);
//       FormatInt(str, value+mod, 16);

//       MoveTo(SCREEN_W/2 + 40 - TextWidth(str)/2, SCREEN_H/2);
//       SetColor(BLACK);
//       Print(str);
//     } else if (diceSelected) {
//       dice->asView.onInput(&dice->asView, input);
//     } else {
//       mod = NumInput(mod, input, &numBounds, true, false);
//     }

//   }

//   Free(dice);
//   Free(btn);
//   HideAllObjs();
//   SetBackground(BG);
//   EraseRect(&wBounds);
//   RedrawPage();
//   ReactivatePage();
// }
