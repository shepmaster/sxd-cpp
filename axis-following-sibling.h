#ifndef AXIS_FOLLOWING_SIBLING_H
#define AXIS_FOLLOWING_SIBLING_H

#include "xpath-internal.h"

class AxisFollowingSibling : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

#endif
