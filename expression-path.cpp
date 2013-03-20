#include "expression-path.h"

ExpressionPath::ExpressionPath(std::vector<std::unique_ptr<XPathExpression>> steps) :
  _steps(move(steps))
{}

XPathValue
ExpressionPath::evaluate(const XPathEvaluationContext &context) const {
  Nodeset result;
  result.add(context.node());

  for (auto &step : _steps) {
    Nodeset step_result;

    auto sub_context = context.new_context_for(result.size());

    for (auto &current_node : result) {
      sub_context->next(current_node);
      auto selected = step->evaluate(*sub_context);
      // TODO: What if it is not a nodeset?
      step_result.add_nodeset(selected.nodeset());
    }

    result = step_result;
  }

  return XPathValue(result);
}
