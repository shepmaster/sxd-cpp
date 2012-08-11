#include "axis-parent.h"

void
AxisParent::traverse(Node *node, const Node::foreach_fn_t &fn) {
  fn(node->parent());
}
