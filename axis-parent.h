#ifndef AXIS_PARENT_H
#define AXIS_PARENT_H

#include "xpath-internal.h"

class AxisParent : public XPathAxis {
public:
  void traverse(Node *node, const Node::foreach_fn_t &fn);
};

#endif
