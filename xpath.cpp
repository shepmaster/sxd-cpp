#include "xpath.h"
#include "xpath-core-function-library.h"

XPath::XPath(std::unique_ptr<XPathExpression> expression) :
  _expression(std::move(expression))
{
}

Nodeset
XPath::select_nodes(Node *node) const
{
  Nodeset empty_nodeset;
  XPathFunctionLibrary functions;
  XPathCoreFunctionLibrary::register_functions(functions);
  XPathEvaluationContext context(node, empty_nodeset, functions);
  return _expression->evaluate(context).nodeset();
}
