#include "expression-variable.h"

ExpressionVariable::ExpressionVariable(std::string name) :
  _name(name)
{}

XPathValue
ExpressionVariable::evaluate(const XPathEvaluationContext &context) const
{
  return context.variable_for_name(_name);
}
