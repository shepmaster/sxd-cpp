#include "xpath-evaluation-context-impl.h"

using std::make_shared;

XPathEvaluationContextImpl::XPathEvaluationContextImpl(unsigned long size,
                                                       const XPathFunctionLibrary &functions,
                                                       const XPathVariableBindings &variables,
                                                       const NamespaceResolver &namespaces) :
  _size(size),
  _position(0),
  _functions(functions),
  _variables(variables),
  _namespaces(namespaces)
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
XPathEvaluationContextImpl::next(Node *context_node)
{
  _node = context_node;
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

const std::string *
XPathEvaluationContextImpl::find_namespace_for_prefix(const std::string prefix) const
{
  return _namespaces.find_namespace_for_prefix(prefix);
}

std::shared_ptr<XPathEvaluationContext>
XPathEvaluationContextImpl::new_context_for(unsigned long size) const
{
  return make_shared<XPathEvaluationContextImpl>(size, _functions, _variables, _namespaces);
}
