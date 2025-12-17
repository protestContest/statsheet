#include "views/spell_detail_view.h"
#include "kit/canvas.h"
#include "kit/input.h"
#include "kit/str.h"
#include "kit/text.h"
#include "kit/vec.h"
#include "ui.h"
#include "views/page_list.h"

static char *LineEnd(char *line, u32 width)
{
  char *word = line;
  u32 wordLen = 0;
  u32 lineLen = 0;

  while (*word && lineLen + wordLen < width) {
    lineLen += wordLen;
    line = word;
    wordLen = 0;

    while (IsSpace(*word)) {
      lineLen += CharWidth(*word);
      word++;
    }
    if (*word == '\n') return word;

    while (*word && !IsWhitespace(*word)) {
      wordLen += CharWidth(*word);
      if (*word == '-') {
        word++;
        break;
      }
      word++;
    }
  }
  if (lineLen + wordLen < width) {
    line = word;
  }

  while (IsSpace(*line)) line++;
  return line;
}

static void DrawSpellDetail(View *view)
{
  FontInfo info;
  GetFontInfo(&info);
  SpellDetailView *spellView = (SpellDetailView*)view;
  DrawWindow(&view->bounds);

  SetColor(BLACK);

  i16 cx = (view->bounds.right + view->bounds.left)/2;
  i16 w = TextWidth(spellView->spell->name);
  MoveTo(cx - w/2, view->bounds.top + 3 + LineHeight());
  Print(spellView->spell->name);

  Rect textRect = {
    view->bounds.left + 5,
    view->bounds.top + 5 + LineHeight(),
    view->bounds.right - 5,
    view->bounds.bottom
  };

  i16 y = textRect.top + info.ascent;
  MoveTo(textRect.left, y);
  u32 line = spellView->curLine;
  char *cur = spellView->spell->description + spellView->lineStarts[line];
  while (*cur && y + info.descent <= textRect.bottom) {
    char *lineEnd = spellView->spell->description + spellView->lineStarts[++line];

    while (cur < lineEnd) {
      DrawChar(*cur);
      cur++;
    }
    y += LineHeight();
    if (*cur == '\n') {
      y += LineHeight()/2;
      while (*cur == '\n') cur++;
    }

    MoveTo(textRect.left, y);
  }
}

static bool InputSpellDetail(View *view, u16 input)
{
  SpellDetailView *spellView = (SpellDetailView*)view;
  if (KeyPressed(BTN_B)) {
    return true;
  } else if (KeyPressed(BTN_UP)) {
    if (spellView->curLine > 5) {
      spellView->curLine -= 5;
    } else {
      spellView->curLine = 0;
    }
    EraseRect(&view->bounds);
    DrawView(view);
  } else if (KeyPressed(BTN_DOWN)) {
    if (spellView->curLine < VecCount(spellView->lineStarts) - 6) {
      spellView->curLine += 5;
    } else {
      spellView->curLine = VecCount(spellView->lineStarts) - 1;
    }
    EraseRect(&view->bounds);
    DrawView(view);
  }
  return false;
}

static void ActivateSpellDetail(View *view, bool active)
{
  if (!active) {
    SetBackground(BG);
    RedrawPage();
  }
}

void InitSpellDetailView(SpellDetailView *view, Rect *bounds, Spell *spell)
{
  InitView(&view->asView, bounds, DrawSpellDetail, InputSpellDetail, ActivateSpellDetail);
  view->spell = spell;
  view->lineStarts = 0;
  view->curLine = 0;

  Rect textRect = {
    bounds->left + 5,
    bounds->top + 5 + LineHeight(),
    bounds->right - 5,
    bounds->bottom
  };

  FontInfo info;
  GetFontInfo(&info);
  char *cur = spell->description;
  VecPush(view->lineStarts, 0);
  while (*cur) {
    cur = LineEnd(cur, textRect.right - textRect.left);
    VecPush(view->lineStarts, cur - spell->description);
    while (*cur == '\n') cur++;
  }
}
