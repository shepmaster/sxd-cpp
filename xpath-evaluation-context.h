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
  unsigned long position() const;
  unsigned long size() const;
  void next();
  std::shared_ptr<XPathFunction> function_for_name(std::string name) const;
  bool has_function(std::string name) const;

  XPathEvaluationContext new_context_for(Node *node, Nodeset nodes) const;

private:
  Node *_node;
  Nodeset _nodes;
  unsigned long _position;
  const XPathFunctionLibrary &_functions;
};

#endif
