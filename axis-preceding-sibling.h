#ifndef AXIS_PRECEDING_SIBLING_H
#define AXIS_PRECEDING_SIBLING_H

#include "xpath-internal.h"

class AxisPrecedingSibling : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

#endif
