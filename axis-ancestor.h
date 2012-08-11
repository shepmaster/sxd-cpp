#ifndef AXIS_ANCESTOR_H
#define AXIS_ANCESTOR_H

#include "xpath-internal.h"

class AxisAncestor : public XPathAxis {
public:
  void traverse(Node *node, const Node::foreach_fn_t &fn);
};

#endif
