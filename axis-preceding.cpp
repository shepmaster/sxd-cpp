#include "downward-recursive-tester.h"
#include "axis-preceding.h"

struct PrecedingSiblingAndAncestorTester {
  PrecedingSiblingAndAncestorTester(const Node::foreach_fn_t &fn) : _fn(fn) {}
  void operator() (Node *node) {
    node->foreach_preceding_sibling(DownwardRecursiveTester(_fn));
    node->foreach_ancestor(*this);
  }
  const Node::foreach_fn_t & _fn;
};

void
AxisPreceding::traverse(Node *node, const Node::foreach_fn_t &fn) {
  node->foreach_preceding_sibling(DownwardRecursiveTester(fn));
  node->foreach_ancestor(PrecedingSiblingAndAncestorTester(fn));
}
