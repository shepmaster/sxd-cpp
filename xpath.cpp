#include "xpath.h"
#include "xpath-core-function-library.h"

XPath::XPath(std::unique_ptr<XPathExpression> expression) :
  _expression(std::move(expression))
{
}

static XPathValue
evaluate(Node *node, XPathExpression &expr)
{
  Nodeset empty_nodeset;
  XPathFunctionLibrary functions;
  XPathCoreFunctionLibrary::register_functions(functions);
  XPathEvaluationContext context(node, empty_nodeset, functions);
  return expr.evaluate(context);
}

Nodeset
XPath::select_nodes(Node *node) const
{
  return evaluate(node, *_expression).nodeset();
}

double
XPath::number() const
{
  return evaluate(nullptr, *_expression).number();
}

bool
XPath::boolean() const
{
  return evaluate(nullptr, *_expression).boolean();
}
