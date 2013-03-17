#include "expression-variable.h"

#include "xpath-parsing-exceptions.h"

ExpressionVariable::ExpressionVariable(std::string name) :
  _name(name)
{}

XPathValue
ExpressionVariable::evaluate(const XPathEvaluationContext &context) const
{
  if (! context.has_variable(_name)) {
    throw UnknownVariableException(_name);
  }

  return context.variable_for_name(_name);
}
