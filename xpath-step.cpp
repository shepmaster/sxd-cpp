#include "xpath-step.h"


XPathStep::XPathStep(std::shared_ptr<XPathAxis> axis,
                     std::shared_ptr<XPathNodeTest> node_test) :
  _axis(axis),
  _node_test(node_test)
{
}

void
XPathStep::select_nodes(const XPathEvaluationContext &context,
                        Nodeset &result)
{
  _axis->select_nodes(context.node(), *_node_test, result);
}

std::ostream &
operator<<(std::ostream &strm, const XPathStep &step)
{
  return strm << "XPathStep (" << *step._axis << *step._node_test << ")";
}
