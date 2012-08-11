#include "axis-parent.h"

void
AxisParent::traverse(Node *node, StepTester &test) {
  test(node->parent());
}
