#include "views/text_view.h"
#include "kit/debug.h"
#include "kit/input.h"
#include "kit/str.h"
#include "kit/text.h"
#include "kit/vec.h"
#include "ui.h"

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

static void DrawTextView(View *view)
{
  TextView *textView = (TextView*)view;
  FontInfo info;
  GetFontInfo(&info);
  SetColor(BLACK);

  Rect textRect = view->bounds;

  i16 y = textRect.top + info.ascent;
  MoveTo(textRect.left, y);

  u32 line = textView->pageStarts[textView->curPage];
  char *cur = textView->text + textView->lineStarts[line];
  while (*cur && y + info.descent < textRect.bottom) {
    char *lineEnd = textView->text + textView->lineStarts[++line];

    while (cur < lineEnd && *cur != '\n') {
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

  if (textView->curPage > 0) {
    ShowUpArrow(view->bounds.right - 2, view->bounds.top-1);
  } else {
    HideArrow(dirUp);
  }
  if (textView->curPage < VecCount(textView->pageStarts) - 1) {
    ShowDownArrow(view->bounds.right - 2, view->bounds.bottom - 4);
  } else {
    HideArrow(dirDown);
  }
}

static bool InputTextView(View *view, u16 input)
{
  TextView *textView = (TextView*)view;

  if (KeyPressed(BTN_UP)) {
    if (textView->curPage > 0) {
      textView->curPage--;
      EraseRect(&view->bounds);
      DrawView(view);
    }
  } else if (KeyPressed(BTN_DOWN)) {
    if (textView->curPage < VecCount(textView->pageStarts)-1) {
      textView->curPage++;
      EraseRect(&view->bounds);
      DrawView(view);
    }
  }
  return false;
}

void InitTextView(TextView *view, Rect *bounds, char *text)
{
  InitView(&view->asView, bounds, DrawTextView, InputTextView, 0, 0);
  view->text = text;
  view->lineStarts = 0;
  view->curPage = 0;
  view->pageStarts = 0;

  FontInfo info;
  GetFontInfo(&info);
  char *cur = text;
  VecPush(view->lineStarts, 0);
  VecPush(view->pageStarts, 0);

  i16 y = bounds->top + LineHeight();
  while (*cur) {
    cur = LineEnd(cur, bounds->right - bounds->left);

    y += LineHeight();
    if (*cur == '\n') {
      y += LineHeight()/2;
      while (*cur == '\n') cur++;
    }
    VecPush(view->lineStarts, cur - text);

    if (y > bounds->bottom) {
      VecPush(view->pageStarts, VecCount(view->lineStarts) - 1);
      y = bounds->top + LineHeight();
    }
  }
}
