#include "predicate-value.h"

PredicateValue PredicateValue::boolean(bool value)
{
  xpath_result_t xpath_value;
  xpath_value.type = XPATH_RESULT_TYPE_BOOLEAN;
  xpath_value.value.boolean = value;
  return PredicateValue(xpath_value);
}

PredicateValue PredicateValue::numeric(double value)
{
  xpath_result_t xpath_value;
  xpath_value.type = XPATH_RESULT_TYPE_NUMERIC;
  xpath_value.value.numeric = value;
  return PredicateValue(xpath_value);
}

PredicateValue::PredicateValue(xpath_result_t value) :
  _value(value)
{
}

xpath_result_t
PredicateValue::eval(xpath_evaluation_context_t *context) {
  return _value;
}
