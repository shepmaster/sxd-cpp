#include "predicate-equals.h"

PredicateEquals::PredicateEquals(XPathPredicate &left, XPathPredicate &right) :
  _left(left), _right(right)
{
}

xpath_result_t
PredicateEquals::eval(xpath_evaluation_context_t *context) {
  xpath_result_t result;
  xpath_result_t lresult = _left.eval(context);
  xpath_result_t rresult = _right.eval(context);

  result.type = XPATH_RESULT_TYPE_BOOLEAN;
  if (lresult.type != rresult.type) {
    abort();
  }
  switch (lresult.type) {
  case XPATH_RESULT_TYPE_BOOLEAN:
    result.value.boolean = lresult.value.boolean == rresult.value.boolean;
    break;
  case XPATH_RESULT_TYPE_NUMERIC:
    result.value.boolean = lresult.value.numeric == rresult.value.numeric;
    break;
  case XPATH_RESULT_TYPE_STRING:
    result.value.boolean = (strcmp(lresult.value.string, rresult.value.string) == 0);
    break;
  case XPATH_RESULT_TYPE_NODESET:
    abort();
    break;
  }

  return result;
}
