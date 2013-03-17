#include "xpath-step.h"


XPathStep::XPathStep(std::shared_ptr<XPathAxis> axis,
                     std::shared_ptr<XPathNodeTest> node_test,
                     std::shared_ptr<XPathExpression> predicate) :
  _axis(axis),
  _node_test(node_test),
  _predicate(predicate)
{
}

bool
include(XPathValue value, XPathEvaluationContext &context)
{
  if (value.is(XPathValue::Type::Number)) {
    return context.position() == value.number();
  } else {
    return value.boolean();
  }
}

void
XPathStep::select_nodes(const XPathEvaluationContext &context,
                        Nodeset &result)
{
  Nodeset selected;
  _axis->select_nodes(context.node(), *_node_test, selected);

  if (! _predicate) {
    result.add_nodeset(selected);
  } else {
    // FIXME: The context node should be the child we are looping on
    auto sub_context = context.new_context_for(context.node(), selected);
    for (auto node : selected) {
      auto value = _predicate->evaluate(*sub_context);

      if (include(value, *sub_context)) {
        result.add(node);
      }

      sub_context->next();
    }
  }
}

std::ostream &
operator<<(std::ostream &strm, const XPathStep &step)
{
  return strm << "XPathStep (" << *step._axis << *step._node_test << ")";
}
