#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

extern "C" {
#include "element.h"
#include "xpath-internal.h"
#include "xpath-functions.h"
#include "test-utilities.h"
}

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

static GArray *
ensure_parameters(GArray *params)
{
  if (params) {
    return params;
  } else {
    return g_array_new(FALSE, FALSE, sizeof(xpath_result_t));
  }
}

static GArray *
string_parameters(const char *first, ...)
{
  GArray *params;
  const char *string;
  va_list args;

  va_start(args, first);
  params = ensure_parameters(NULL);

  string = first;
  while (string) {
    xpath_result_t param;
    param.type = XPATH_RESULT_TYPE_STRING;
    param.value.string = strdup(string);

    g_array_append_val(params, param);

    string = va_arg(args, const char *);
  }

  va_end(args);

  return params;
}

static GArray *
add_number_param(GArray *params, double value)
{
  xpath_result_t num;

  params = ensure_parameters(params);

  num.type = XPATH_RESULT_TYPE_NUMERIC;
  num.value.numeric = value;
  g_array_append_val(params, num);

  return params;
}

static GArray *
add_boolean_param(GArray *params, int value)
{
  xpath_result_t num;

  params = ensure_parameters(params);

  num.type = XPATH_RESULT_TYPE_BOOLEAN;
  num.value.boolean = value;
  g_array_append_val(params, num);

  return params;
}

/**
 * This function consumes the nodeset, so the nodeset should not be
 * freed after calling this function
 */
static GArray *
add_nodeset_param(GArray *params, nodeset_t *value)
{
  xpath_result_t num;

  params = ensure_parameters(params);

  num.type = XPATH_RESULT_TYPE_NODESET;
  num.value.nodeset = value;
  g_array_append_val(params, num);

  return params;
}

static void
cleanup_params(GArray *params)
{
  unsigned int i;

  for (i = 0; i < params->len; i++) {
      xpath_result_t *param;
      param = &g_array_index(params, xpath_result_t, i);
      xpath_result_destroy(param);
    }
  g_array_free(params, TRUE);
}

/* 4.1 - Node set functions */

TEST_GROUP(xpath)
{
  /* Most functions take parameters, all return a result */
  GArray *parameters;
  xpath_result_t res;

  document_t *doc;
  node_t *el_1;
  node_t *el_2;
  node_t *el_3;

  void setup(void)
  {
    doc = document_new();
    el_1 = test_helper_new_node(doc, "first");
    el_2 = test_helper_new_node(doc, "second");
    el_3 = test_helper_new_node(doc, "third");
  }

  void teardown(void)
  {
    xpath_result_destroy(&res);
    if (parameters) cleanup_params(parameters);
    node_free(el_1);
    node_free(el_2);
    node_free(el_3);
    document_free(doc);
  }
};

TEST(xpath, fn_last)
{
  nodeset_t *ns;
  xpath_evaluation_context_t context;

  ns = nodeset_new_with_nodes(el_1, el_2, el_3, NULL);

  context.nodeset = ns;

  context.node = el_1;
  res = xpath_fn_last(&context, NULL);
  CHECK_RESULT_NUMERIC(res, 3);

  context.node = el_2;
  res = xpath_fn_last(&context, NULL);
  CHECK_RESULT_NUMERIC(res, 3);

  context.node = el_3;
  res = xpath_fn_last(&context, NULL);
  CHECK_RESULT_NUMERIC(res, 3);

  nodeset_free(ns);
}

TEST(xpath, fn_position)
{
  nodeset_t *ns;
  xpath_evaluation_context_t context;

  ns = nodeset_new_with_nodes(el_1, el_2, el_3, NULL);

  context.nodeset = ns;

  context.node = el_1;
  res = xpath_fn_position(&context, NULL);
  CHECK_RESULT_NUMERIC(res, 1);

  context.node = el_2;
  res = xpath_fn_position(&context, NULL);
  CHECK_RESULT_NUMERIC(res, 2);

  context.node = el_3;
  res = xpath_fn_position(&context, NULL);
  CHECK_RESULT_NUMERIC(res, 3);

  nodeset_free(ns);
}

TEST(xpath, fn_count)
{
  nodeset_t *ns;

  ns = nodeset_new_with_nodes(el_1, el_2, el_3, NULL);
  parameters = add_nodeset_param(NULL, ns);

  res = xpath_fn_count(NULL, parameters);
  CHECK_RESULT_NUMERIC(res, 3);
}

/* 4.2 - String functions */

TEST(xpath, fn_concat_2)
{
  parameters = string_parameters("one", "two", NULL);

  res = xpath_fn_concat(NULL, parameters);
  CHECK_RESULT_STRING(res, "onetwo");
}

TEST(xpath, fn_concat_3)
{
  parameters = string_parameters("one", "two", "three", NULL);

  res = xpath_fn_concat(NULL, parameters);
  CHECK_RESULT_STRING(res, "onetwothree");
}

TEST(xpath, fn_starts_with)
{
  parameters = string_parameters("hello world", "hello", NULL);

  res = xpath_fn_starts_with(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_starts_with_failure)
{
  parameters = string_parameters("hello world", "cow");
  res = xpath_fn_starts_with(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

TEST(xpath, fn_contains)
{
  parameters = string_parameters("hello world", "world", NULL);
  res = xpath_fn_contains(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_contains_failure)
{
  parameters = string_parameters("hello world", "cow", NULL);

  res = xpath_fn_contains(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

TEST(xpath, fn_substring_before)
{
  parameters = string_parameters("1999/04/01", "/", NULL);

  res = xpath_fn_substring_before(NULL, parameters);
  CHECK_RESULT_STRING(res, "1999");
}

TEST(xpath, fn_substring_before_failure)
{
  parameters = string_parameters("hello world", "cow", NULL);

  res = xpath_fn_substring_before(NULL, parameters);
  CHECK_RESULT_STRING(res, "");
}

TEST(xpath, fn_substring_after)
{
  parameters = string_parameters("1999/04/01", "/", NULL);

  res = xpath_fn_substring_after(NULL, parameters);
  CHECK_RESULT_STRING(res, "04/01");
}

TEST(xpath, fn_substring_after_failure)
{
  parameters = string_parameters("hello world", "cow", NULL);

  res = xpath_fn_substring_after(NULL, parameters);
  CHECK_RESULT_STRING(res, "");
}

TEST(xpath, fn_substring_2)
{
  parameters = string_parameters("12345", NULL);
  add_number_param(parameters, 2);

  res = xpath_fn_substring(NULL, parameters);
  CHECK_RESULT_STRING(res, "2345");
}

TEST(xpath, fn_substring_3)
{
  parameters = string_parameters("12345", NULL);
  add_number_param(add_number_param(parameters, 2), 3);

  res = xpath_fn_substring(NULL, parameters);
  CHECK_RESULT_STRING(res, "234");
}

TEST(xpath, fn_substring_unicode)
{
  parameters = string_parameters("ウィキペディア", NULL);
  add_number_param(add_number_param(parameters, 2), 3);

  res = xpath_fn_substring(NULL, parameters);
  CHECK_RESULT_STRING(res, "ィキペ");
}

/* 4.3 - Boolean functions */

TEST(xpath, fn_boolean_boolean)
{
  parameters = add_boolean_param(NULL, TRUE);
  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_boolean_number)
{
  parameters = add_number_param(NULL, 3);
  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_boolean_number_negative_zero)
{
  parameters = add_number_param(NULL, -0.0);
  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

TEST(xpath, fn_boolean_number_nan)
{
  parameters = add_number_param(NULL, NAN);
  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

TEST(xpath, fn_boolean_nodeset)
{
  nodeset_t *ns;

  ns = nodeset_new_with_nodes(el_1, el_2, el_3, NULL);
  parameters = add_nodeset_param(NULL, ns);

  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_boolean_nodeset_empty)
{
  nodeset_t *ns;

  ns = nodeset_new();
  parameters = add_nodeset_param(NULL, ns);

  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

TEST(xpath, fn_boolean_string)
{
  parameters = string_parameters("hello world", NULL);
  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_boolean_string_empty)
{
  parameters = string_parameters("", NULL);
  res = xpath_fn_boolean(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

TEST(xpath, fn_not_true)
{
  parameters = add_boolean_param(NULL, TRUE);
  res = xpath_fn_not(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

TEST(xpath, fn_not_false)
{
  parameters = add_boolean_param(NULL, FALSE);
  res = xpath_fn_not(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_true)
{
  res = xpath_fn_true(NULL, NULL);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_false)
{
  res = xpath_fn_false(NULL, NULL);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

/* 4.4 - Number functions */

TEST(xpath, fn_floor)
{
  xpath_result_t *value;

  parameters = g_array_new(FALSE, FALSE, sizeof(*value));
  g_array_set_size(parameters, 1);
  value = &g_array_index(parameters, xpath_result_t, 0);

  value->type = XPATH_RESULT_TYPE_NUMERIC;

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

  parameters = g_array_new(FALSE, FALSE, sizeof(*value));
  g_array_set_size(parameters, 1);
  value = &g_array_index(parameters, xpath_result_t, 0);

  value->type = XPATH_RESULT_TYPE_NUMERIC;

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

  parameters = g_array_new(FALSE, FALSE, sizeof(*value));
  g_array_set_size(parameters, 1);
  value = &g_array_index(parameters, xpath_result_t, 0);

  value->type = XPATH_RESULT_TYPE_NUMERIC;

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
