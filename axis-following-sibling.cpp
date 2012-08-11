#include "axis-following-sibling.h"

void
AxisFollowingSibling::traverse(Node *node, StepTester &test) {
  node->foreach_following_sibling(std::ref(test));
}
