#ifndef AXIS_DESCENDANT_H
#define AXIS_DESCENDANT_H

#include "xpath-internal.h"

class AxisDescendant : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

#endif
