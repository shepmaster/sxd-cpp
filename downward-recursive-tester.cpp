#include "downward-recursive-tester.h"

DownwardRecursiveTester::DownwardRecursiveTester(Node::foreach_fn_t fn) :
  _fn(fn)
{
}

void
DownwardRecursiveTester::operator() (Node *node) {
  _fn(node);
  node->foreach_child(*this);
}
