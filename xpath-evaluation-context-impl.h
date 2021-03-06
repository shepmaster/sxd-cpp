#ifndef XPATH_EVALUATION_CONTEXT_IMPL_H
#define XPATH_EVALUATION_CONTEXT_IMPL_H

#include "xpath-evaluation-context.h"

class XPathEvaluationContextImpl : public XPathEvaluationContext
{
public:
  XPathEvaluationContextImpl(unsigned long size,
                             const XPathFunctionLibrary &functions,
                             const XPathVariableBindings &variables,
                             const NamespaceResolver &namespaces);
  Node *node() const;
  unsigned long position() const;
  unsigned long size() const;
  void next(Node *new_context_node);

  bool has_function(std::string name) const;

  std::shared_ptr<XPathFunction>
  function_for_name(std::string name) const;

  bool has_variable(std::string name) const;

  XPathValue
  variable_for_name(std::string name) const;

  const std::string *
  find_namespace_for_prefix(const std::string prefix) const;

  std::shared_ptr<XPathEvaluationContext>
  new_context_for(unsigned long size) const;

private:
  Node *_node;
  unsigned long _size;
  unsigned long _position;
  const XPathFunctionLibrary &_functions;
  const XPathVariableBindings &_variables;
  const NamespaceResolver &_namespaces;
};

#endif
