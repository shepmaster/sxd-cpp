#include "downward-recursive-tester.h"
#include "axis-preceding.h"

struct PrecedingSiblingAndAncestorTester {
  PrecedingSiblingAndAncestorTester(StepTester &test) : _test(test) {}
  void operator() (Node *node) {
    node->foreach_preceding_sibling(DownwardRecursiveTester(_test));
    node->foreach_ancestor(std::ref(*this));
  }
  StepTester &_test;
};

void
AxisPreceding::traverse(Node *node, StepTester &test) {
  node->foreach_preceding_sibling(DownwardRecursiveTester(test));
  node->foreach_ancestor(PrecedingSiblingAndAncestorTester(test));
}
