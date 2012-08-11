#ifndef XPATH_AXIS_H
#define XPATH_AXIS_H

#include "node.h"

class XPathAxis {
public:
  virtual ~XPathAxis() {};
  virtual void traverse(Node *node, const Node::foreach_fn_t &fn) = 0;
};

#endif
