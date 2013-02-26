#include "expression-function.h"

#include "xpath-function.h"
#include "xpath-parsing-exceptions.h"

ExpressionFunction::ExpressionFunction(std::string name,
                                       std::vector<std::shared_ptr<XPathExpression>> arguments) :
  _name(name),
  _arguments(std::move(arguments))
{
}

std::vector<XPathValue>
evaluate_arguments(const std::vector<std::shared_ptr<XPathExpression>> &expressions,
                   const XPathEvaluationContext &context)
{
  std::vector<XPathValue> arguments;
  for (auto &expression : expressions) {
    arguments.push_back(expression->evaluate(context));
  }
  return arguments;
}

XPathValue
ExpressionFunction::evaluate(const XPathEvaluationContext &context) const
{
  if (! context.has_function(_name)) {
    throw UnknownXPathFunctionException(_name);
  }

  auto fn = context.function_for_name(_name);
  // TODO: Error when argument count mismatch
  auto arguments = evaluate_arguments(_arguments, context);
  return fn->evaluate(context, arguments);
}
