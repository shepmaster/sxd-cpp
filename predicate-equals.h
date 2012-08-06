#ifndef PREDICATE_EQUALS_H
#define PREDICATE_EQUALS_H

#include "xpath-internal.h"

class PredicateEquals : public XPathPredicate {
public:
  PredicateEquals(XPathPredicate &left, XPathPredicate &right);

  xpath_result_t eval(xpath_evaluation_context_t *context);

private:
  XPathPredicate &_left;
  XPathPredicate &_right;
};

#endif
