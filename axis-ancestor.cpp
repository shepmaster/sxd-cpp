#include "axis-ancestor.h"

void
AxisAncestor::traverse(Node *node, StepTester &test) {
  node->foreach_ancestor(std::ref(test));
}
