#ifndef AXIS_CHILD_H
#define AXIS_CHILD_H

#include "xpath-internal.h"

class AxisChild : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

#endif
