#include "axis-ancestor-or-self.h"

void
AxisAncestorOrSelf::traverse(Node *node, StepTester &test) {
  test(node);
  node->foreach_ancestor(std::ref(test));
}
