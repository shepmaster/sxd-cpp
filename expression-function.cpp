#include "expression-function.h"

ExpressionFunction::ExpressionFunction(std::string name) :
  _name(name)
{
}

XPathValue
ExpressionFunction::evaluate(const XPathEvaluationContext &context) const
{
  auto fn = context.function_for_name(_name);
  // TODO: Error when there is no such function
  return fn->evaluate();
}
