#ifndef AXIS_PRECEDING_H
#define AXIS_PRECEDING_H

#include "xpath-internal.h"

class AxisPreceding : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

#endif
