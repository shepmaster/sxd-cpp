#ifndef AXIS_FOLLOWING_H
#define AXIS_FOLLOWING_H

#include "xpath-internal.h"

class AxisFollowing : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

#endif
