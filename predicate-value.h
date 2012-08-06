#ifndef PREDICATE_VALUE_H
#define PREDICATE_VALUE_H

#include "xpath-internal.h"

class PredicateValue : public XPathPredicate {
public:
  static PredicateValue boolean(bool value);
  static PredicateValue numeric(double value);
  PredicateValue(xpath_result_t value);

  xpath_result_t eval(xpath_evaluation_context_t *context);

private:
  xpath_result_t _value;
};

#endif
