#include "axis-ancestor-or-self.h"

void
AxisAncestorOrSelf::traverse(Node *node, const Node::foreach_fn_t &fn) {
  fn(node);
  node->foreach_ancestor(fn);
}
