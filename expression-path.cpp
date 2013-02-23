#include "expression-path.h"

ExpressionPath::ExpressionPath(std::vector<std::unique_ptr<XPathStep>> steps) :
  _steps(move(steps))
{}

XPathValue
ExpressionPath::evaluate(const XPathEvaluationContext &context) const {
  Nodeset result;
  result.add(context.node());

  for (auto &step : _steps) {
    Nodeset step_result;

    for (auto i = 0; i < result.count(); i++) {
      auto *current_node = result[i];
      auto sub_context = context.new_context_for(current_node, step_result);
      step->select_nodes(sub_context, step_result);
    }

    result = step_result;
  }

  return XPathValue(result);
}
