#ifndef AXIS_CHILD_H
#define AXIS_CHILD_H

#include "xpath-axis.h"

class AxisChild : public XPathAxis {
public:
  void traverse(Node *node, const Node::foreach_fn_t &fn);
};

#endif
