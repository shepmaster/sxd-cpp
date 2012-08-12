#include "node-test-element.h"

bool
NodeTestElement::include_node(Node &node)
{
  return node.type() == NODE_TYPE_ELEMENT;
}
