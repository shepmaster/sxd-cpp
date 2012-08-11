#include "axis-self.h"

void
AxisSelf::traverse(Node *node, StepTester &test) {
  test(node);
}
