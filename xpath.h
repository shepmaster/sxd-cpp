#ifndef XPATH_H
#define XPATH_H

#include <string>
#include "nodeset.h"
#include "node.h"
#include "xpath-variable-bindings-hash.h"

#include "xpath-expression.h"

class XPath : public NodeSelector {
public:
  XPath(std::unique_ptr<XPathExpression> expression);

  void bind_variable(std::string name, XPathValue value);
  Nodeset select_nodes(Node *node) const;

  double number() const;
  bool boolean() const;
  Nodeset nodeset() const;

private:
  std::unique_ptr<XPathExpression> _expression;
  std::unique_ptr<XPathVariableBindingsHash> _variables;
  std::unique_ptr<NamespaceResolver> _null_namespaces;
};

#endif
