#include "views/spell_detail_view.h"
#include "kit/canvas.h"
#include "kit/input.h"
#include "kit/text.h"
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
}

static bool InputSpellDetail(View *view, u16 input)
{
  SpellDetailView *spellView = (SpellDetailView*)view;
  if (KeyPressed(BTN_B)) {
    HideArrows();
    return true;
  } else {
    InputView(&spellView->content.asView, input);
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
    bounds->bottom - 5
  };
  InitTextView(&view->content, &contentBounds, spell->description);
}
