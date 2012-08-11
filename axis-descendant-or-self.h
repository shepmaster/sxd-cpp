#ifndef AXIS_DESCENDANT_OR_SELF_H
#define AXIS_DESCENDANT_OR_SELF_H

#include "xpath-axis.h"

class AxisDescendantOrSelf : public XPathAxis {
public:
  void traverse(Node *node, const Node::foreach_fn_t &fn);
};

#endif
