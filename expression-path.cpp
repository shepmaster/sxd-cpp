#include "expression-path.h"

ExpressionPath::ExpressionPath(std::vector<std::unique_ptr<XPathExpression>> steps,
                               std::vector<std::unique_ptr<XPathExpression>> predicates) :
  _steps(move(steps)),
  _predicates(move(predicates))
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
ExpressionPath::evaluate(const XPathEvaluationContext &context) const {
  Nodeset result;
  result.add(context.node());

  for (auto step_i = 0; step_i < _steps.size(); ++step_i) {
    auto &step = _steps[step_i];
    auto &predicate = _predicates[step_i];

    Nodeset step_result;

    for (auto i = 0; i < result.count(); i++) {
      auto *current_node = result[i];
      auto sub_context = context.new_context_for(current_node, step_result);
      auto selected = step->evaluate(*sub_context);
      // TODO: What if it is not a nodeset?
      step_result.add_nodeset(selected.nodeset());
    }

    if (predicate) {
      Nodeset selected;

      // FIXME: The context node should be the child we are looping on
      auto sub_context = context.new_context_for(nullptr, step_result); // nullptr is right out

      for (auto *current_node : step_result) {
        auto value = predicate->evaluate(*sub_context);

        if (include(value, *sub_context)) {
          selected.add(current_node);
        }

        sub_context->next();
      }

      step_result = selected;
    }

    result = step_result;
  }

  return XPathValue(result);
}
