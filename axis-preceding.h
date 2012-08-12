#ifndef AXIS_PRECEDING_H
#define AXIS_PRECEDING_H

#include "xpath-axis.h"

class AxisPreceding : public XPathAxis {
public:
  void traverse(Node *node, const Node::foreach_fn_t &fn);
};

#endif