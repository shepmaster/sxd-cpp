#ifndef XPATH_EVALUATION_CONTEXT_H
#define XPATH_EVALUATION_CONTEXT_H

class XPathFunction;

#include "nodeset.h"
#include "xpath-function-library.h"

class XPathEvaluationContext
{
public:
  XPathEvaluationContext(Node *node, Nodeset nodes, const XPathFunctionLibrary &functions);
  Node *node() const;
  unsigned long position();
  unsigned long size() const;
  void next();
  std::shared_ptr<XPathFunction> function_for_name(std::string name) const;

private:
  Node *_node;
  Nodeset _nodes;
  unsigned long _position;
  const XPathFunctionLibrary &_functions;
};

#endif
