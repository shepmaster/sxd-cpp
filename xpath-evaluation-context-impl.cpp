#include "xpath-evaluation-context-impl.h"

using std::make_shared;

XPathEvaluationContextImpl::XPathEvaluationContextImpl(Node *node,
                                                       unsigned long size,
                                                       const XPathFunctionLibrary &functions,
                                                       const XPathVariableBindings &variables) :
  _node(node),
  _size(size),
  _position(1),
  _functions(functions),
  _variables(variables)
{
}

Node *
XPathEvaluationContextImpl::node() const
{
  return _node;
}

unsigned long
XPathEvaluationContextImpl::position() const
{
  return _position;
}

unsigned long
XPathEvaluationContextImpl::size() const
{
  return _size;
}

void
XPathEvaluationContextImpl::next()
{
  _position++;
}

bool
XPathEvaluationContextImpl::has_function(std::string name) const
{
  return _functions.has_function(name);
}

std::shared_ptr<XPathFunction>
XPathEvaluationContextImpl::function_for_name(std::string name) const
{
  return _functions.function_for_name(name);
}

bool
XPathEvaluationContextImpl::has_variable(std::string name) const
{
  return _variables.has_value(name);
}

XPathValue
XPathEvaluationContextImpl::variable_for_name(std::string name) const
{
  return _variables.value_for(name);
}

std::shared_ptr<XPathEvaluationContext>
XPathEvaluationContextImpl::new_context_for(Node *node, unsigned long size) const
{
  return make_shared<XPathEvaluationContextImpl>(node, size, _functions, _variables);
}
