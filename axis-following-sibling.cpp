#include "axis-following-sibling.h"

void
AxisFollowingSibling::traverse(Node *node, const Node::foreach_fn_t &fn) {
  node->foreach_following_sibling(fn);
}
