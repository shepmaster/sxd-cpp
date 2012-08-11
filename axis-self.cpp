#include "axis-self.h"

void
AxisSelf::traverse(Node *node, const Node::foreach_fn_t &fn) {
  fn(node);
}
