#ifndef DOWNWARD_RECURSIVE_TESTER_H
#define DOWNWARD_RECURSIVE_TESTER_H

#include "xpath-internal.h"

struct DownwardRecursiveTester {
  DownwardRecursiveTester(StepTester &test);
  void operator() (Node *node);
  StepTester &_test;
};

#endif
