#include "xpath-step.h"


XPathStep::XPathStep(std::shared_ptr<XPathAxis> axis,
                     std::shared_ptr<XPathNodeTest> node_test,
                     std::shared_ptr<XPathExpression> predicate) :
  _axis(axis),
  _node_test(node_test),
  _predicate(predicate)
{
}

void
XPathStep::select_nodes(Node *current_node, Nodeset &result)
{
  Nodeset selected;
  _axis->select_nodes(current_node, *_node_test, selected);

  if (! _predicate) {
    result.add_nodeset(selected);
  } else {
    XPathEvaluationContext context(selected);
    for (auto node : selected) {
      auto value = _predicate->evaluate(context);

      if (context.position() == value.number()) {
        result.add(node);
      }

      context.next();
    }
  }
}

std::ostream &
operator<<(std::ostream &strm, const XPathStep &step)
{
  return strm << "XPathStep (" << *step._axis << *step._node_test << ")";
}
