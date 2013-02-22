#include "xpath-evaluation-context.h"

XPathEvaluationContext::XPathEvaluationContext(Node *node, Nodeset nodes,
                                               const XPathFunctionLibrary &functions) :
  _node(node),
  _nodes(nodes),
  _position(1),
  _functions(functions)
{
}

Node *
XPathEvaluationContext::node() const
{
  return _node;
}

unsigned long
XPathEvaluationContext::position() const
{
  return _position;
}

unsigned long
XPathEvaluationContext::size() const
{
  return _nodes.size();
}

void
XPathEvaluationContext::next()
{
  _position++;
}

std::shared_ptr<XPathFunction>
XPathEvaluationContext::function_for_name(std::string name) const
{
  return _functions.function_for_name(name);
}

XPathEvaluationContext
XPathEvaluationContext::new_context_for(Node *node, Nodeset nodes) const
{
  return XPathEvaluationContext(node, nodes, _functions);
}
