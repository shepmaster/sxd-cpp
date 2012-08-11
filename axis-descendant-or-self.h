#ifndef AXIS_DESCENDANT_OR_SELF_H
#define AXIS_DESCENDANT_OR_SELF_H

#include "xpath-internal.h"

class AxisDescendantOrSelf : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

#endif
