#include "axis-preceding-sibling.h"

void
AxisPrecedingSibling::traverse(Node *node, const Node::foreach_fn_t &fn) {
  node->foreach_preceding_sibling(fn);
}
