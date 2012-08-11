#include "downward-recursive-tester.h"
#include "axis-following.h"

struct FollowingSiblingAndAncestorTester {
  FollowingSiblingAndAncestorTester(StepTester &test) : _test(test) {}
  void operator() (Node *node) {
    node->foreach_following_sibling(DownwardRecursiveTester(_test));
    node->foreach_ancestor(*this);
  }
  StepTester &_test;
};

void
AxisFollowing::traverse(Node *node, StepTester &test) {
  node->foreach_following_sibling(DownwardRecursiveTester(test));
  node->foreach_ancestor(FollowingSiblingAndAncestorTester(test));
}
