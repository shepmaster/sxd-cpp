#ifndef AXIS_ANCESTOR_OR_SELF_H
#define AXIS_ANCESTOR_OR_SELF_H

#include "xpath-internal.h"

class AxisAncestorOrSelf : public XPathAxis {
public:
  void traverse(Node *node, const Node::foreach_fn_t &fn);
};

#endif
