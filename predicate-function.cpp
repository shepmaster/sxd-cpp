#include "predicate-function.h"

PredicateFunction::PredicateFunction(xpath_fn_t fn, xpath_parameters_t params) :
  _fn(fn), _params(params)
{
}

xpath_result_t
PredicateFunction::eval(xpath_evaluation_context_t *context) {
  return _fn(context, _params);
}
