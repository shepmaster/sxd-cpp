#include "downward-recursive-tester.h"

DownwardRecursiveTester::DownwardRecursiveTester(StepTester &test) :
  _test(test)
{
}

void
DownwardRecursiveTester::operator() (Node *node) {
  _test(node);
  node->foreach_child(*this);
}
