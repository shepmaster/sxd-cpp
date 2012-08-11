#include "axis-ancestor.h"

void
AxisAncestor::traverse(Node *node, const Node::foreach_fn_t &fn) {
  node->foreach_ancestor(fn);
}
