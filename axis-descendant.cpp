#include "downward-recursive-tester.h"
#include "axis-descendant.h"

void
AxisDescendant::traverse(Node *node, StepTester &test) {
  node->foreach_child(DownwardRecursiveTester(test));
}
