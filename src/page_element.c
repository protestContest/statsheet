#include "page_element.h"

void InitPageElement(PageElement *el, Rect *bounds, ViewDraw draw, ViewInput onInput)
{
  InitView(&el->asView, bounds, draw, onInput, 0);
  el->left = 0;
  el->right = 0;
  el->up = 0;
  el->down = 0;
}

void LinkElementBelow(PageElement *above, PageElement *below)
{
  if (above->down && above->down->up == above) {
    above->down->up = 0;
  }
  if (below->up && below->up->down == below) {
    below->up->down = 0;
  }
  above->down = below;
  below->up = above;
}

void LinkElementBeside(PageElement *left, PageElement *right)
{
  if (left->right && left->right->left == left) {
    left->right->left = 0;
  }
  if (right->left && right->left->right == right) {
    right->left->right = 0;
  }
  left->right = right;
  right->left = left;
}
