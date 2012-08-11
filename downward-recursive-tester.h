#ifndef DOWNWARD_RECURSIVE_TESTER_H
#define DOWNWARD_RECURSIVE_TESTER_H

#include "xpath-internal.h"

struct DownwardRecursiveTester {
  DownwardRecursiveTester(Node::foreach_fn_t fn);
  void operator() (Node *node);
  Node::foreach_fn_t _fn;
};

#endif
