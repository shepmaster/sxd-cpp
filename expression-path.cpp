#include "expression-path.h"

ExpressionPath::ExpressionPath(std::unique_ptr<XPathExpression> start_point,
                               std::vector<std::unique_ptr<XPathExpression>> steps) :
  _start_point(move(start_point)),
  _steps(move(steps))
{}

XPathValue
ExpressionPath::evaluate(const XPathEvaluationContext &context) const {
  Nodeset result = _start_point->evaluate(context).nodeset();

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
