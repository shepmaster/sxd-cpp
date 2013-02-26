#ifndef XPATH_H
#define XPATH_H

#include <string>
#include "nodeset.h"
#include "node.h"

#include "xpath-expression.h"

class XPath : public NodeSelector {
public:
  XPath(std::unique_ptr<XPathExpression> expression);

  Nodeset select_nodes(Node *node) const;
  double number() const;
  bool boolean() const;

private:
  std::unique_ptr<XPathExpression> _expression;
};

#endif
