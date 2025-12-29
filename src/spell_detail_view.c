#include "views/spell_detail_view.h"
#include "kit/canvas.h"
#include "kit/input.h"
#include "kit/text.h"
#include "spell.h"
#include "ui.h"
#include "view.h"
#include "views/page_list.h"
#include "views/text_view.h"

static void DrawSpellDetail(View *view)
{
  SpellDetailView *spellView = (SpellDetailView*)view;
  DrawWindow(&view->bounds);
  DrawView(&spellView->title.asView);
  DrawView(&spellView->content.asView);
  DrawView(&spellView->prepBtn.asView);
  DrawView(&spellView->castBtn.asView);
  MoveTo(view->bounds.left+3, view->bounds.bottom - 5 - 16);
  LineTo(view->bounds.right-4, view->bounds.bottom - 5 - 16);
}

static bool InputSpellDetail(View *view, u16 input)
{
  SpellDetailView *spellView = (SpellDetailView*)view;
  if (KeyPressed(BTN_B)) {
    HideArrows();
    return true;
  } else if (KeyPressed(BTN_LEFT)) {
    spellView->selected = &spellView->prepBtn.asView;
    SelectView(spellView->selected);
  } else if (KeyPressed(BTN_RIGHT)) {
    spellView->selected = &spellView->castBtn.asView;
    SelectView(spellView->selected);
  } else if (KeyPressed(BTN_A)) {
    if (spellView->selected == &spellView->castBtn.asView) {
      // cast spell
    } else {
      // prepare spell
    }
  } else {
    InputView(&spellView->content.asView, input);
  }

  return false;
}

static void ActivateSpellDetail(View *view, bool active)
{
  SpellDetailView *spellView = (SpellDetailView*)view;
  if (active) {
    PlaceCursor(spellView->selected->bounds.left, spellView->selected->bounds.top + 7);
    ShowCursor();
  } else {
    SetBackground(BG);
    RedrawPage();
  }
}

static void DestroySpellDetail(View *view)
{
  SpellDetailView *spellView = (SpellDetailView*)view;
  DestroyView(&spellView->content.asView);
}

void InitSpellDetailView(SpellDetailView *view, Rect *bounds, Spell *spell)
{
  InitView(&view->asView, bounds, DrawSpellDetail, InputSpellDetail, ActivateSpellDetail, DestroySpellDetail);
  view->spell = spell;

  Rect titleBounds = *bounds;
  InsetRect(&titleBounds, 3, 3);
  titleBounds.bottom = titleBounds.top + LineHeight();
  InitSpellRow(&view->title, &titleBounds, spell);
  view->title.highligted = true;

  Rect contentBounds = {
    bounds->left + 5,
    bounds->top + 5 + LineHeight(),
    bounds->right - 5,
    bounds->bottom - 5 - 20
  };
  InitTextView(&view->content, &contentBounds, spell->description);

  Rect btnBounds = {bounds->right - 5 - 40, bounds->bottom - 3 - 16,  bounds->right - 5, bounds->bottom - 3};
  InitLabel(&view->castBtn, &btnBounds, "Cast");

  OffsetRect(&btnBounds, -50 - 10, 0);
  btnBounds.right += 10;
  char *prepLabel = IsSpellPrepared(spell->name) ? "Unprepare" : "Prepare";
  InitLabel(&view->prepBtn, &btnBounds, prepLabel);

  view->selected = &view->castBtn.asView;
}
