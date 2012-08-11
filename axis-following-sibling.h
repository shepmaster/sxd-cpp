#ifndef AXIS_FOLLOWING_SIBLING_H
#define AXIS_FOLLOWING_SIBLING_H

#include "xpath-axis.h"

class AxisFollowingSibling : public XPathAxis {
public:
  void traverse(Node *node, const Node::foreach_fn_t &fn);
};

#endif
