#include "expression-path.h"

ExpressionPath::ExpressionPath(std::vector<std::unique_ptr<XPathExpression>> steps) :
  _steps(move(steps))
{}

XPathValue
ExpressionPath::evaluate(const XPathEvaluationContext &context) const {
  Nodeset result;
  result.add(context.node());

  for (auto step_i = 0; step_i < _steps.size(); ++step_i) {
    auto &step = _steps[step_i];

    Nodeset step_result;

    for (auto i = 0; i < result.count(); i++) {
      auto *current_node = result[i];
      auto sub_context = context.new_context_for(current_node, step_result.size());
      auto selected = step->evaluate(*sub_context);
      // TODO: What if it is not a nodeset?
      step_result.add_nodeset(selected.nodeset());
    }

    result = step_result;
  }

  return XPathValue(result);
}
