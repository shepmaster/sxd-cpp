#ifndef AXIS_SELF_H
#define AXIS_SELF_H

#include "xpath-internal.h"

class AxisSelf : public XPathAxis {
public:
  void traverse(Node *node, const Node::foreach_fn_t &fn);
};

#endif
