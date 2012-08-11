#include "downward-recursive-tester.h"
#include "axis-descendant.h"

void
AxisDescendant::traverse(Node *node, const Node::foreach_fn_t &fn) {
  node->foreach_child(DownwardRecursiveTester(fn));
}
