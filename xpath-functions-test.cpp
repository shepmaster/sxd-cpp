#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "element.h"
#include "xpath-internal.h"
#include "xpath-functions.h"
#include "test-utilities.h"

/* XPath 1.0 */

#define CHECK_RESULT_NUMERIC(_res, _value)	       \
{						       \
  CHECK_EQUAL(XPATH_RESULT_TYPE_NUMERIC, (_res).type); \
  CHECK_EQUAL((_value), (_res).value.numeric);	       \
}

#define CHECK_RESULT_BOOLEAN(_res, _value)		\
{							\
  CHECK_EQUAL(XPATH_RESULT_TYPE_BOOLEAN, (_res).type);	\
  CHECK_EQUAL((_value), (_res).value.boolean);		\
}

#define CHECK_RESULT_STRING(_res, _value)		\
{							\
  CHECK_EQUAL(XPATH_RESULT_TYPE_STRING, (_res).type);	\
  STRCMP_EQUAL((_value), (_res).value.string);		\
}

static void
add_string_parameters(xpath_parameters_t &params, const char *first, ...)
{
  const char *string;
  va_list args;

  va_start(args, first);

  string = first;
  while (string) {
    xpath_result_t param;
    param.type = XPATH_RESULT_TYPE_STRING;
    param.value.string = strdup(string);

    params.push_back(param);

    string = va_arg(args, const char *);
  }

  va_end(args);
}

static void
add_number_param(xpath_parameters_t &params, double value)
{
  xpath_result_t num;

  num.type = XPATH_RESULT_TYPE_NUMERIC;
  num.value.numeric = value;
  params.push_back(num);
}

static void
add_boolean_param(xpath_parameters_t &params, int value)
{
  xpath_result_t num;

  num.type = XPATH_RESULT_TYPE_BOOLEAN;
  num.value.boolean = value;
  params.push_back(num);
}

/**
 * This function consumes the nodeset, so the nodeset should not be
 * freed after calling this function
 */
static void
add_nodeset_param(xpath_parameters_t &params, Nodeset *value)
{
  xpath_result_t num;

  num.type = XPATH_RESULT_TYPE_NODESET;
  num.value.nodeset = value;
  params.push_back(num);
}

static void
cleanup_params(xpath_parameters_t &params)
{
  unsigned int i;

  for (i = 0; i < params.size(); i++) {
      xpath_result_t *param = &params[i];
      xpath_result_destroy(param);
    }
}

/* 4.1 - Node set functions */

TEST_GROUP(xpath)
{
  /* Most functions take parameters, all return a result */
  xpath_parameters_t parameters;
  xpath_result_t res;

  Document *doc;
  Node *el_1;
  Node *el_2;
  Node *el_3;

  void setup(void)
  {
    doc = new Document();
    el_1 = test_helper_new_node(*doc, "first");
    el_2 = test_helper_new_node(*doc, "second");
    el_3 = test_helper_new_node(*doc, "third");
  }

  void teardown(void)
  {
    xpath_result_destroy(&res);
    cleanup_params(parameters);
    delete el_1;
    delete el_2;
    delete el_3;
    delete doc;
  }
};

TEST(xpath, fn_last)
{
  Nodeset *ns;
  xpath_evaluation_context_t context;

  ns = nodeset_new_with_nodes(el_1, el_2, el_3, NULL);

  context.nodeset = ns;

  context.node = el_1;
  res = xpath_fn_last(&context, parameters);
  CHECK_RESULT_NUMERIC(res, 3);

  context.node = el_2;
  res = xpath_fn_last(&context, parameters);
  CHECK_RESULT_NUMERIC(res, 3);

  context.node = el_3;
  res = xpath_fn_last(&context, parameters);
  CHECK_RESULT_NUMERIC(res, 3);

  delete ns;
}

TEST(xpath, fn_position)
{
  Nodeset *ns;
  xpath_evaluation_context_t context;

  ns = nodeset_new_with_nodes(el_1, el_2, el_3, NULL);

  context.nodeset = ns;

  context.node = el_1;
  res = xpath_fn_position(&context, parameters);
  CHECK_RESULT_NUMERIC(res, 1);

  context.node = el_2;
  res = xpath_fn_position(&context, parameters);
  CHECK_RESULT_NUMERIC(res, 2);

  context.node = el_3;
  res = xpath_fn_position(&context, parameters);
  CHECK_RESULT_NUMERIC(res, 3);

  delete ns;
}

TEST(xpath, fn_count)
{
  Nodeset *ns;

  ns = nodeset_new_with_nodes(el_1, el_2, el_3, NULL);
  add_nodeset_param(parameters, ns);

  res = xpath_fn_count(NULL, parameters);
  CHECK_RESULT_NUMERIC(res, 3);
}

/* 4.2 - String functions */

TEST(xpath, fn_concat_2)
{
  add_string_parameters(parameters, "one", "two", NULL);

  res = xpath_fn_concat(NULL, parameters);
  CHECK_RESULT_STRING(res, "onetwo");
}

TEST(xpath, fn_concat_3)
{
  add_string_parameters(parameters, "one", "two", "three", NULL);

  res = xpath_fn_concat(NULL, parameters);
  CHECK_RESULT_STRING(res, "onetwothree");
}

TEST(xpath, fn_starts_with)
{
  add_string_parameters(parameters, "hello world", "hello", NULL);

  res = xpath_fn_starts_with(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_starts_with_failure)
{
  add_string_parameters(parameters, "hello world", "cow", NULL);
  res = xpath_fn_starts_with(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

TEST(xpath, fn_contains)
{
  add_string_parameters(parameters, "hello world", "world", NULL);
  res = xpath_fn_contains(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_contains_failure)
{
  add_string_parameters(parameters, "hello world", "cow", NULL);

  res = xpath_fn_contains(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

TEST(xpath, fn_substring_before)
{
  add_string_parameters(parameters, "1999/04/01", "/", NULL);

  res = xpath_fn_substring_before(NULL, parameters);
  CHECK_RESULT_STRING(res, "1999");
}

TEST(xpath, fn_substring_before_failure)
{
  add_string_parameters(parameters, "hello world", "cow", NULL);

  res = xpath_fn_substring_before(NULL, parameters);
  CHECK_RESULT_STRING(res, "");
}

TEST(xpath, fn_substring_after)
{
  add_string_parameters(parameters, "1999/04/01", "/", NULL);

  res = xpath_fn_substring_after(NULL, parameters);
  CHECK_RESULT_STRING(res, "04/01");
}

TEST(xpath, fn_substring_after_failure)
{
  add_string_parameters(parameters, "hello world", "cow", NULL);

  res = xpath_fn_substring_after(NULL, parameters);
  CHECK_RESULT_STRING(res, "");
}

TEST(xpath, fn_substring_2)
{
  add_string_parameters(parameters, "12345", NULL);
  add_number_param(parameters, 2);

  res = xpath_fn_substring(NULL, parameters);
  CHECK_RESULT_STRING(res, "2345");
}

TEST(xpath, fn_substring_3)
{
  add_string_parameters(parameters, "12345", NULL);
  add_number_param(parameters, 2);
  add_number_param(parameters, 3);

  res = xpath_fn_substring(NULL, parameters);
  CHECK_RESULT_STRING(res, "234");
}

TEST(xpath, fn_substring_unicode)
{
  add_string_parameters(parameters, "ウィキペディア", NULL);
  add_number_param(parameters, 2);
  add_number_param(parameters, 3);

  res = xpath_fn_substring(NULL, parameters);
  CHECK_RESULT_STRING(res, "ィキペ");
}

/* 4.3 - Boolean functions */

TEST(xpath, fn_boolean_boolean)
{
  add_boolean_param(parameters, TRUE);
  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_boolean_number)
{
  add_number_param(parameters, 3);
  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_boolean_number_negative_zero)
{
  add_number_param(parameters, -0.0);
  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

TEST(xpath, fn_boolean_number_nan)
{
  add_number_param(parameters, NAN);
  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

TEST(xpath, fn_boolean_nodeset)
{
  Nodeset *ns;

  ns = nodeset_new_with_nodes(el_1, el_2, el_3, NULL);
  add_nodeset_param(parameters, ns);

  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_boolean_nodeset_empty)
{
  Nodeset *ns;

  ns = new Nodeset();
  add_nodeset_param(parameters, ns);

  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

TEST(xpath, fn_boolean_string)
{
  add_string_parameters(parameters, "hello world", NULL);
  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_boolean_string_empty)
{
  add_string_parameters(parameters, "", NULL);
  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

TEST(xpath, fn_not_true)
{
  add_boolean_param(parameters, TRUE);
  res = xpath_fn_not(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

TEST(xpath, fn_not_false)
{
  add_boolean_param(parameters, FALSE);
  res = xpath_fn_not(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_true)
{
  res = xpath_fn_true(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_false)
{
  res = xpath_fn_false(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

/* 4.4 - Number functions */

TEST(xpath, fn_floor)
{
  xpath_result_t *value;

  // Create a dummy value
  add_number_param(parameters, 0);
  value = &parameters[0];

  value->value.numeric = 3.5;
  res = xpath_fn_floor(NULL, parameters);
  CHECK_RESULT_NUMERIC(res, 3);

  value->value.numeric = 0;
  res = xpath_fn_floor(NULL, parameters);
  CHECK_RESULT_NUMERIC(res, 0);

  value->value.numeric = -9.9;
  res = xpath_fn_floor(NULL, parameters);
  CHECK_RESULT_NUMERIC(res, -10);
}

TEST(xpath, fn_ceiling)
{
  xpath_result_t *value;

  // Create a dummy value
  add_number_param(parameters, 0);
  value = &parameters[0];

  value->value.numeric = 3.5;
  res = xpath_fn_ceiling(NULL, parameters);
  CHECK_RESULT_NUMERIC(res, 4);

  value->value.numeric = 0;
  res = xpath_fn_ceiling(NULL, parameters);
  CHECK_RESULT_NUMERIC(res, 0);

  value->value.numeric = -9.9;
  res = xpath_fn_ceiling(NULL, parameters);
  CHECK_RESULT_NUMERIC(res, -9);
}

TEST(xpath, fn_round)
{
  xpath_result_t *value;

  // Add a dummy value
  add_number_param(parameters, 0);
  value = &parameters[0];

#define CHECK_round(_in, _expected)			\
  value->value.numeric = _in;				\
  res = xpath_fn_round(NULL, parameters);		\
  CHECK_RESULT_NUMERIC(res, _expected);			\

  CHECK_round(2.0, 2);
  CHECK_round(1.9, 2);
  CHECK_round(1.5, 2);
  CHECK_round(1.1, 1);
  CHECK_round(1.0, 1);
  CHECK_round(0.9, 1);
  CHECK_round(0.5, 1);
  CHECK_round(0.1, 0);
  CHECK_round(-0.1, -0);
  CHECK_round(-0.5, -0);
  CHECK_round(-0.9, -1);
  CHECK_round(-1.0, -1);
  CHECK_round(-1.1, -1);
  CHECK_round(-1.5, -1);
  CHECK_round(-1.9, -2);
  CHECK_round(-2.0, -2);

  CHECK_round(INFINITY, INFINITY);
  CHECK_round(-INFINITY, -INFINITY);
  CHECK_round(-0, -0);
  CHECK_round(0, 0);

#undef CHECK_round

  value->value.numeric = NAN;
  res = xpath_fn_round(NULL, parameters);
  CHECK_EQUAL(XPATH_RESULT_TYPE_NUMERIC, res.type);
  CHECK(isnan(res.value.numeric));
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
