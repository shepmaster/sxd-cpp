#ifndef PREDICATE_FUNCTION_H
#define PREDICATE_FUNCTION_H

#include "xpath-internal.h"

class PredicateFunction : public XPathPredicate {
public:
  PredicateFunction(xpath_fn_t fn, xpath_parameters_t params);

  xpath_result_t eval(xpath_evaluation_context_t *context);

private:
  xpath_fn_t _fn;
  xpath_parameters_t _params;
};

#endif
