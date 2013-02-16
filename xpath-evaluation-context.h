#ifndef XPATH_EVALUATION_CONTEXT_H
#define XPATH_EVALUATION_CONTEXT_H

#include "nodeset.h"

class XPathEvaluationContext
{
public:
  XPathEvaluationContext(Nodeset nodes);
  unsigned long position();
  void next();

private:
  unsigned long _position;
};

#endif
