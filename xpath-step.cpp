#include "xpath-step.h"

XPathStep::XPathStep(std::unique_ptr<XPathAxis> axis, std::unique_ptr<XPathNodeTest> node_test) :
  _axis(std::move(axis)), _node_test(std::move(node_test))
{
}

void
XPathStep::select_nodes(Node *current_node, Nodeset &result)
{
  _axis->select_nodes(current_node, *_node_test, result);
}

std::ostream &
operator<<(std::ostream &strm, const XPathStep &step)
{
  return strm << "XPathStep (" << *step._axis << *step._node_test << ")";
}
