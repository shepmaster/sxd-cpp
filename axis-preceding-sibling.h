#ifndef AXIS_PRECEDING_SIBLING_H
#define AXIS_PRECEDING_SIBLING_H

#include "xpath-axis.h"

class AxisPrecedingSibling : public XPathAxis {
public:
  void traverse(Node *node, const Node::foreach_fn_t &fn);
};

#endif
