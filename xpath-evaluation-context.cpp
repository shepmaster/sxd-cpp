#include "xpath-evaluation-context.h"

XPathEvaluationContext::XPathEvaluationContext(Nodeset nodes) :
  _position(1)
{
}

unsigned long
XPathEvaluationContext::position()
{
  return _position;
}

void XPathEvaluationContext::next()
{
  _position++;
}
