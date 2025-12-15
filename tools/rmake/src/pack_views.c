#include "pack_views.h"
#include "parse.h"
#include "vec.h"
#include "../../inc/view.h"
#include <string.h>

/*
Binary format for pages:

For each page:
  u8 viewPage;           // enum value (0)
  char[] title;          // null-terminated
  u32 numViews;
  For each view:
    u8 viewType;         // enum
    i32[4] bounds;       // left, top, right, bottom
    [view params]        // varies by type
    i32[4] links;        // left, top, right, bottom

View params by type:
  viewButton: char[] title
  viewLabel: char[] title
  viewNumStat: char[] title, char[] statName, char[] editStatName
  viewChargeStat: char[] title, char[] statName, char[] maxStatName
  viewNumControl: i32 value, i32 flags
  viewChargeControl: i32 value, i32 maxValue
*/

typedef struct {
  ViewType type;
  Rect bounds;
  char *strA, *strB, *strC;
  i32 intA, intB;
  Rect links;
} ViewInfo;

typedef struct {
  char *title;
  ViewInfo *views;
} PageInfo;

static char *ParseQuotedString(char **cur, char *end)
{
  if (*cur >= end || **cur != '"') return NULL;
  (*cur)++;
  char *start = *cur;
  while (*cur < end && **cur != '"') (*cur)++;
  u32 len = *cur - start;
  char *str = malloc(len + 1);
  memcpy(str, start, len);
  str[len] = 0;
  if (*cur < end) (*cur)++;  // skip closing quote
  return str;
}

static bool ParseRect(char **cur, char *end, Rect *rect)
{
  SkipSpaces(*cur, end);
  if (*cur >= end || **cur != '(') return false;
  (*cur)++;

  char *numStart;
  SkipSpaces(*cur, end);
  numStart = *cur;
  while (*cur < end && (IsDigit(**cur) || **cur == '-')) (*cur)++;
  rect->left = atoi(numStart);

  SkipSpaces(*cur, end);
  if (*cur < end && **cur == ',') (*cur)++;

  SkipSpaces(*cur, end);
  numStart = *cur;
  while (*cur < end && (IsDigit(**cur) || **cur == '-')) (*cur)++;
  rect->top = atoi(numStart);

  SkipSpaces(*cur, end);
  if (*cur < end && **cur == ',') (*cur)++;

  SkipSpaces(*cur, end);
  numStart = *cur;
  while (*cur < end && (IsDigit(**cur) || **cur == '-')) (*cur)++;
  rect->right = atoi(numStart);

  SkipSpaces(*cur, end);
  if (*cur < end && **cur == ',') (*cur)++;

  SkipSpaces(*cur, end);
  numStart = *cur;
  while (*cur < end && (IsDigit(**cur) || **cur == '-')) (*cur)++;
  rect->bottom = atoi(numStart);

  SkipSpaces(*cur, end);
  if (*cur < end && **cur == ')') (*cur)++;

  return true;
}

static void SkipToEndOfLine(char **cur, char *end)
{
  while (*cur < end && **cur != '\n') (*cur)++;
  if (*cur < end) (*cur)++;
}

static bool MatchKeyword(char **cur, char *end, const char *keyword)
{
  u32 len = strlen(keyword);
  if (*cur + len > end) return false;
  if (strncmp(*cur, keyword, len) != 0) return false;
  if (*cur + len < end && IsSymChar((*cur)[len])) return false;
  *cur += len;
  return true;
}

static PageInfo *ParseViewsFile(FILE *f, u32 size)
{
  PageInfo *pages = NULL;
  char *data = malloc(size);
  fread(data, size, 1, f);

  char *end = data + size;
  char *cur = SkipWhitespace(data, end);

  while (cur < end) {
    if (MatchKeyword(&cur, end, "page")) {
      PageInfo page = {0};
      SkipSpaces(cur, end);
      page.title = ParseQuotedString(&cur, end);
      if (!page.title) {
        fprintf(stderr, "Expected page title\n");
        exit(1);
      }
      VecPush(pages, page);
      cur = SkipWhitespace(cur, end);
    }
    else if (MatchKeyword(&cur, end, "numStat")) {
      if (!pages || VecCount(pages) == 0) {
        fprintf(stderr, "View defined before page\n");
        exit(1);
      }
      ViewInfo view = {0};
      view.type = viewNumStat;
      SkipSpaces(cur, end);
      ParseRect(&cur, end, &view.bounds);
      SkipSpaces(cur, end);
      view.strA = ParseQuotedString(&cur, end);  // title
      SkipSpaces(cur, end);
      view.strB = ParseQuotedString(&cur, end);  // statName
      SkipSpaces(cur, end);
      view.strC = ParseQuotedString(&cur, end);  // editStatName
      VecPush(pages[VecCount(pages)-1].views, view);
      SkipToEndOfLine(&cur, end);
      cur = SkipWhitespace(cur, end);
    }
    else if (MatchKeyword(&cur, end, "chargeStat")) {
      if (!pages || VecCount(pages) == 0) {
        fprintf(stderr, "View defined before page\n");
        exit(1);
      }
      ViewInfo view = {0};
      view.type = viewChargeStat;
      SkipSpaces(cur, end);
      ParseRect(&cur, end, &view.bounds);
      SkipSpaces(cur, end);
      view.strA = ParseQuotedString(&cur, end);  // title
      SkipSpaces(cur, end);
      view.strB = ParseQuotedString(&cur, end);  // statName
      SkipSpaces(cur, end);
      view.strC = ParseQuotedString(&cur, end);  // maxStatName
      VecPush(pages[VecCount(pages)-1].views, view);
      SkipToEndOfLine(&cur, end);
      cur = SkipWhitespace(cur, end);
    }
    else if (MatchKeyword(&cur, end, "button")) {
      if (!pages || VecCount(pages) == 0) {
        fprintf(stderr, "View defined before page\n");
        exit(1);
      }
      ViewInfo view = {0};
      view.type = viewButton;
      SkipSpaces(cur, end);
      ParseRect(&cur, end, &view.bounds);
      SkipSpaces(cur, end);
      view.strA = ParseQuotedString(&cur, end);  // title
      VecPush(pages[VecCount(pages)-1].views, view);
      SkipToEndOfLine(&cur, end);
      cur = SkipWhitespace(cur, end);
    }
    else if (MatchKeyword(&cur, end, "label")) {
      if (!pages || VecCount(pages) == 0) {
        fprintf(stderr, "View defined before page\n");
        exit(1);
      }
      ViewInfo view = {0};
      view.type = viewLabel;
      SkipSpaces(cur, end);
      ParseRect(&cur, end, &view.bounds);
      SkipSpaces(cur, end);
      view.strA = ParseQuotedString(&cur, end);  // title
      VecPush(pages[VecCount(pages)-1].views, view);
      SkipToEndOfLine(&cur, end);
      cur = SkipWhitespace(cur, end);
    }
    else if (MatchKeyword(&cur, end, "numControl")) {
      if (!pages || VecCount(pages) == 0) {
        fprintf(stderr, "View defined before page\n");
        exit(1);
      }
      ViewInfo view = {0};
      view.type = viewNumControl;
      SkipSpaces(cur, end);
      ParseRect(&cur, end, &view.bounds);
      SkipSpaces(cur, end);
      // Parse value and flags
      char *numStart = cur;
      while (cur < end && (IsDigit(*cur) || *cur == '-')) cur++;
      view.intA = atoi(numStart);
      SkipSpaces(cur, end);
      numStart = cur;
      while (cur < end && (IsDigit(*cur) || *cur == '-')) cur++;
      view.intB = atoi(numStart);
      VecPush(pages[VecCount(pages)-1].views, view);
      SkipToEndOfLine(&cur, end);
      cur = SkipWhitespace(cur, end);
    }
    else if (MatchKeyword(&cur, end, "chargeControl")) {
      if (!pages || VecCount(pages) == 0) {
        fprintf(stderr, "View defined before page\n");
        exit(1);
      }
      ViewInfo view = {0};
      view.type = viewChargeControl;
      SkipSpaces(cur, end);
      ParseRect(&cur, end, &view.bounds);
      SkipSpaces(cur, end);
      // Parse value and maxValue
      char *numStart = cur;
      while (cur < end && (IsDigit(*cur) || *cur == '-')) cur++;
      view.intA = atoi(numStart);
      SkipSpaces(cur, end);
      numStart = cur;
      while (cur < end && (IsDigit(*cur) || *cur == '-')) cur++;
      view.intB = atoi(numStart);
      VecPush(pages[VecCount(pages)-1].views, view);
      SkipToEndOfLine(&cur, end);
      cur = SkipWhitespace(cur, end);
    }
    else if (MatchKeyword(&cur, end, "link")) {
      if (!pages || VecCount(pages) == 0) {
        fprintf(stderr, "Link defined before page\n");
        exit(1);
      }
      PageInfo *page = &pages[VecCount(pages)-1];
      if (!page->views || VecCount(page->views) == 0) {
        fprintf(stderr, "Link defined before any view\n");
        exit(1);
      }
      ViewInfo *view = &page->views[VecCount(page->views)-1];
      SkipSpaces(cur, end);
      ParseRect(&cur, end, &view->links);
      SkipToEndOfLine(&cur, end);
      cur = SkipWhitespace(cur, end);
    }
    else if (*cur == '#') {
      // Skip comment
      SkipToEndOfLine(&cur, end);
      cur = SkipWhitespace(cur, end);
    }
    else {
      // Skip unknown content
      SkipToEndOfLine(&cur, end);
      cur = SkipWhitespace(cur, end);
    }
  }

  free(data);
  return pages;
}

static u32 StrLen(char *s)
{
  return s ? strlen(s) + 1 : 1;  // include null terminator
}

void PackViews(ResInfo *info, FILE *f)
{
  PageInfo *pages = ParseViewsFile(f, info->size);

  info->size = sizeof(u32);  // header
  for (u32 p = 0; p < VecCount(pages); p++) {
    PageInfo *page = &pages[p];
    info->size += sizeof(u8);                // viewPage enum
    info->size += StrLen(page->title);       // title string
    info->size += sizeof(u32);               // numViews

    for (u32 v = 0; v < VecCount(page->views); v++) {
      ViewInfo *view = &page->views[v];
      info->size += sizeof(u8);              // view type
      info->size += sizeof(Rect);            // bounds

      switch (view->type) {
        case viewButton:
        case viewLabel:
          info->size += StrLen(view->strA);
          break;
        case viewNumStat:
        case viewChargeStat:
          info->size += StrLen(view->strA);
          info->size += StrLen(view->strB);
          info->size += StrLen(view->strC);
          break;
        case viewNumControl:
        case viewChargeControl:
          info->size += sizeof(i32) * 2;
          break;
        default:
          break;
      }

      info->size += sizeof(Rect);            // links
    }
  }

  // Allocate and write binary data
  info->data = malloc(info->size);
  *((u32*)info->data) = (info->size - sizeof(u32)) << 8;
  u8 *cur = info->data + sizeof(u32);

  for (u32 p = 0; p < VecCount(pages); p++) {
    PageInfo *page = &pages[p];

    // Write page header
    *cur++ = viewPage;

    // Write title
    u32 titleLen = StrLen(page->title);
    if (page->title) {
      memcpy(cur, page->title, titleLen);
    } else {
      *cur = 0;
    }
    cur += titleLen;

    // Write numViews
    u32 numViews = VecCount(page->views);
    memcpy(cur, &numViews, sizeof(numViews));
    cur += sizeof(numViews);

    // Write each view
    for (u32 v = 0; v < numViews; v++) {
      ViewInfo *view = &page->views[v];

      // Write view type
      *cur++ = view->type;

      // Write bounds
      memcpy(cur, &view->bounds, sizeof(Rect));
      cur += sizeof(Rect);

      // Write view-specific params
      switch (view->type) {
        case viewButton:
        case viewLabel: {
          u32 len = StrLen(view->strA);
          if (view->strA) {
            memcpy(cur, view->strA, len);
          } else {
            *cur = 0;
          }
          cur += len;
          break;
        }
        case viewNumStat:
        case viewChargeStat: {
          u32 lenA = StrLen(view->strA);
          u32 lenB = StrLen(view->strB);
          u32 lenC = StrLen(view->strC);
          if (view->strA) memcpy(cur, view->strA, lenA);
          else *cur = 0;
          cur += lenA;
          if (view->strB) memcpy(cur, view->strB, lenB);
          else *cur = 0;
          cur += lenB;
          if (view->strC) memcpy(cur, view->strC, lenC);
          else *cur = 0;
          cur += lenC;
          break;
        }
        case viewNumControl:
        case viewChargeControl:
          memcpy(cur, &view->intA, sizeof(i32));
          cur += sizeof(i32);
          memcpy(cur, &view->intB, sizeof(i32));
          cur += sizeof(i32);
          break;
        default:
          break;
      }

      // Write links
      memcpy(cur, &view->links, sizeof(Rect));
      cur += sizeof(Rect);
    }
  }

  // Free parsed data
  for (u32 p = 0; p < VecCount(pages); p++) {
    free(pages[p].title);
    for (u32 v = 0; v < VecCount(pages[p].views); v++) {
      free(pages[p].views[v].strA);
      free(pages[p].views[v].strB);
      free(pages[p].views[v].strC);
    }
    FreeVec(pages[p].views);
  }
  FreeVec(pages);

  fprintf(stderr, "Packed views: %d bytes\n", info->size);
}
