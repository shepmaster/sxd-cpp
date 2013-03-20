#include "expression-predicate.h"

ExpressionPredicate::ExpressionPredicate(SubExpression node_selecting_expression,
                                         SubExpression predicate) :
  _node_selecting_expression(node_selecting_expression),
  _predicate(predicate)
{}

static bool
include(XPathValue value, XPathEvaluationContext &context)
{
  if (value.is(XPathValue::Type::Number)) {
    return context.position() == value.number();
  } else {
    return value.boolean();
  }
}

XPathValue
ExpressionPredicate::evaluate(const XPathEvaluationContext &context) const
{
  Nodeset selected;

  auto value = _node_selecting_expression->evaluate(context);
  Nodeset nodes = value.nodeset();
  // TODO: What if not a nodeset?

  // TODO: The context node should be the child we are looping on
  auto sub_context = context.new_context_for(nullptr, nodes); // nullptr is right out

  for (auto *current_node : nodes) {
    auto value = _predicate->evaluate(*sub_context);

    if (include(value, *sub_context)) {
      selected.add(current_node);
    }

    sub_context->next();
  }

  return selected;
}
