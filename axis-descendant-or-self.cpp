#include "downward-recursive-tester.h"
#include "axis-descendant-or-self.h"

void
AxisDescendantOrSelf::traverse(Node *node, StepTester &test) {
  test(node);
  node->foreach_child(DownwardRecursiveTester(test));
}
