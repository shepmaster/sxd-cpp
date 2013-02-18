#include "xpath-evaluation-context.h"

XPathEvaluationContext::XPathEvaluationContext(Nodeset nodes,
                                               const XPathFunctionLibrary &functions) :
  _position(1), _functions(functions)
{
}

unsigned long
XPathEvaluationContext::position()
{
  return _position;
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
