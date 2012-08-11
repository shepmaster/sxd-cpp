#include "axis-preceding-sibling.h"

void
AxisPrecedingSibling::traverse(Node *node, StepTester &test) {
  node->foreach_preceding_sibling(std::ref(test));
}
