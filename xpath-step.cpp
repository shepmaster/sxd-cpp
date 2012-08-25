#include "xpath-step.h"

XPathStep::XPathStep(std::unique_ptr<XPathAxis> axis) :
  _axis(std::move(axis))
{
}

void
XPathStep::select_nodes(Node *current_node, Nodeset &result)
{
  _axis->select_nodes(current_node, result);
}
