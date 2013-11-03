#include "xpath.h"
#include "xpath-core-function-library.h"
#include "xpath-evaluation-context-impl.h"
#include "null-namespace-resolver.h"

#include "make-unique.h"

XPath::XPath(std::unique_ptr<XPathExpression> expression) :
  _expression(std::move(expression)),
  _variables(make_unique<XPathVariableBindingsHash>()),
  _null_namespaces(make_unique<NullNamespaceResolver>())
{
}

static XPathValue
evaluate(Node *node, XPathExpression &expr,
         const XPathVariableBindings &variables,
         const NamespaceResolver &namespaces)
{
  XPathFunctionLibrary functions;
  XPathCoreFunctionLibrary::register_functions(functions);
  XPathEvaluationContextImpl context(1, functions, variables, namespaces);
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
  auto *resolver = dynamic_cast<NamespaceResolver *>(node);
  if (! resolver) {
    resolver = _null_namespaces.get();
  }

  return evaluate(node, *_expression, *_variables, *resolver).nodeset();
}

double
XPath::number() const
{
  return evaluate(nullptr, *_expression, *_variables, *_null_namespaces).number();
}

bool
XPath::boolean() const
{
  return evaluate(nullptr, *_expression, *_variables, *_null_namespaces).boolean();
}

Nodeset
XPath::nodeset() const
{
  return evaluate(nullptr, *_expression, *_variables, *_null_namespaces).nodeset();
}
