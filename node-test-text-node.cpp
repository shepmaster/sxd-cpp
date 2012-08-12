#include "node-test-text-node.h"

bool
NodeTestTextNode::include_node(Node &node)
{
  return node.type() == NODE_TYPE_TEXT_NODE;
}
