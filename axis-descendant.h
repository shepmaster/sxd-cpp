#ifndef AXIS_DESCENDANT_H
#define AXIS_DESCENDANT_H

#include "xpath-axis.h"

class AxisDescendant : public XPathAxis {
public:
  void traverse(Node *node, const Node::foreach_fn_t &fn);
};

#endif
