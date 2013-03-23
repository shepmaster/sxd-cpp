#include "xpath.h"
#include "xpath-core-function-library.h"
#include "xpath-evaluation-context-impl.h"

#include "make-unique.h"

XPath::XPath(std::unique_ptr<XPathExpression> expression) :
  _expression(std::move(expression)),
  _variables(make_unique<XPathVariableBindingsHash>())
{
}

static XPathValue
evaluate(Node *node, XPathExpression &expr, const XPathVariableBindings &variables)
{
  XPathFunctionLibrary functions;
  XPathCoreFunctionLibrary::register_functions(functions);
  XPathEvaluationContextImpl context(1, functions, variables);
  context.next(node);
  return expr.evaluate(context);
}

void
XPath::bind_variable(std::string name, XPathValue value)
{
  _variables->set(name, value);
}

Nodeset
XPath::select_nodes(Node *node) const
{
  return evaluate(node, *_expression, *_variables).nodeset();
}

double
XPath::number() const
{
  return evaluate(nullptr, *_expression, *_variables).number();
}

bool
XPath::boolean() const
{
  return evaluate(nullptr, *_expression, *_variables).boolean();
}

Nodeset
XPath::nodeset() const
{
  return evaluate(nullptr, *_expression, *_variables).nodeset();
}
