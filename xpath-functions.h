#ifndef XPATH_FUNCTIONS_H
#define XPATH_FUNCTIONS_H

/* XPath 1.0 Core Function Library */

/* 4.1 - Node Set Functions */

xpath_result_t
xpath_fn_last(xpath_evaluation_context_t *context, xpath_parameters_t &parameters_unused);

xpath_result_t
xpath_fn_position(xpath_evaluation_context_t *context, xpath_parameters_t &parameters_unused);

xpath_result_t
xpath_fn_count(xpath_evaluation_context_t *context_unused, xpath_parameters_t &parameters);

/* 4.2 - String Functions */

xpath_result_t
xpath_fn_concat(xpath_evaluation_context_t *context_unused, xpath_parameters_t &parameters);

xpath_result_t
xpath_fn_starts_with(xpath_evaluation_context_t *context_unused, xpath_parameters_t &parameters);

xpath_result_t
xpath_fn_contains(xpath_evaluation_context_t *context_unused, xpath_parameters_t &parameters);

xpath_result_t
xpath_fn_substring_before(xpath_evaluation_context_t *context_unused, xpath_parameters_t &parameters);

xpath_result_t
xpath_fn_substring_after(xpath_evaluation_context_t *context_unused, xpath_parameters_t &parameters);

xpath_result_t
xpath_fn_substring(xpath_evaluation_context_t *context_unused, xpath_parameters_t &parameters);

/* 4.3 - Boolean Functions */

xpath_result_t
xpath_fn_boolean(xpath_evaluation_context_t *context_unused, xpath_parameters_t &parameters);

xpath_result_t
xpath_fn_not(xpath_evaluation_context_t *context_unused, xpath_parameters_t &parameters);

xpath_result_t
xpath_fn_true(xpath_evaluation_context_t *context_unused, xpath_parameters_t &parameters_unused);

xpath_result_t
xpath_fn_false(xpath_evaluation_context_t *context_unused, xpath_parameters_t &parameters_unused);

/* 4.4 - Number Functions */

xpath_result_t
xpath_fn_floor(xpath_evaluation_context_t *context_unused, xpath_parameters_t &parameters);

xpath_result_t
xpath_fn_ceiling(xpath_evaluation_context_t *context_unused, xpath_parameters_t &parameters);

xpath_result_t
xpath_fn_round(xpath_evaluation_context_t *context_unused, xpath_parameters_t &parameters);

#endif
