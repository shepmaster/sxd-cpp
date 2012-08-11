#include "downward-recursive-tester.h"
#include "axis-descendant-or-self.h"

void
AxisDescendantOrSelf::traverse(Node *node, const Node::foreach_fn_t &fn) {
  fn(node);
  node->foreach_child(DownwardRecursiveTester(fn));
}
