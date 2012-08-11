#include "downward-recursive-tester.h"
#include "axis-following.h"

struct FollowingSiblingAndAncestorTester {
  FollowingSiblingAndAncestorTester(const Node::foreach_fn_t &fn) : _fn(fn) {}
  void operator() (Node *node) {
    node->foreach_following_sibling(DownwardRecursiveTester(_fn));
    node->foreach_ancestor(*this);
  }
  const Node::foreach_fn_t & _fn;
};

void
AxisFollowing::traverse(Node *node, const Node::foreach_fn_t &fn) {
  node->foreach_following_sibling(DownwardRecursiveTester(fn));
  node->foreach_ancestor(FollowingSiblingAndAncestorTester(fn));
}
