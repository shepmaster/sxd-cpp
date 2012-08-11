#include "axis-child.h"

void
AxisChild::traverse(Node *node, StepTester &test) {
  node->foreach_child(std::ref(test));
}
