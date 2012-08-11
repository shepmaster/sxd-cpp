#ifndef AXIS_CHILD_H
#define AXIS_CHILD_H

#include "xpath-internal.h"

class AxisChild : public XPathAxis {
public:
  void traverse(Node *node, const Node::foreach_fn_t &fn);
};

#endif
