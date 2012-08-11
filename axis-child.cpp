#include "axis-child.h"

void
AxisChild::traverse(Node *node, const Node::foreach_fn_t &fn) {
  node->foreach_child(fn);
}
