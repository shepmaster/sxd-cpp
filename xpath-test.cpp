#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

extern "C" {
#include "xpath-internal.h"
#include "test-utilities.h"
}

static xpath_predicate_t g_predicate_value_true;
static xpath_predicate_t g_predicate_value_false;

TEST_GROUP(xpath)
{};

void
dump_xpath_tokens(xpath_tokens_t *tokens)
{
  unsigned int i;
  for (i = 0; i < tokens->tokens->len; i++) {
    xpath_token_t *t = &g_array_index(tokens->tokens, xpath_token_t, i);
    printf("%d\n", t->start);
  }
}

#define CHECK_token(_tokens, _index, _type, _start)			\
  {									\
    xpath_token_t *__token;						\
    __token = &g_array_index(_tokens->tokens, xpath_token_t, _index);	\
    CHECK_EQUAL(_type, __token->type);					\
    CHECK_EQUAL(_start, __token->start);				\
}

TEST(xpath, tokenize)
{
  xpath_tokens_t *tokens;
  const char * const xpath = "//one/two";
  tokens = xpath_tokenize(xpath);

  STRCMP_EQUAL(xpath, tokens->xpath);
  CHECK_EQUAL(5, tokens->tokens->len);

  CHECK_token(tokens, 0, SLASH, 0);
  CHECK_token(tokens, 1, SLASH, 1);
  CHECK_token(tokens, 2, TEXT,  2);
  CHECK_token(tokens, 3, SLASH, 5);
  CHECK_token(tokens, 4, TEXT,  6);

  xpath_tokens_free(tokens);
}

#define CHECK_str(_tokens, _index, _str)		\
  {							\
    char *__str = xpath_tokens_string(_tokens, _index);	\
    STRCMP_EQUAL(_str, __str);				\
    free(__str);					\
  }							\

TEST(xpath, tokens_string)
{
  xpath_tokens_t *tokens;
  const char * const xpath = "//one/two";

  tokens = xpath_tokenize(xpath);
  CHECK_str(tokens, 0, "/");
  CHECK_str(tokens, 1, "/");
  CHECK_str(tokens, 2, "one");
  CHECK_str(tokens, 3, "/");
  CHECK_str(tokens, 4, "two");

  xpath_tokens_free(tokens);
}

TEST(xpath, compile_element)
{
  const char * const name = "one";
  xpath_compiled_t *compiled;

  compiled = xpath_compile(name);

  CHECK_EQUAL(1, compiled->steps->len);
  CHECK_EQUAL(XPATH_AXIS_CHILD, g_array_index(compiled->steps, xpath_step_t, 0).axis);
  CHECK_EQUAL(XPATH_NODE_TYPE_ELEMENT, g_array_index(compiled->steps, xpath_step_t, 0).type);
  STRCMP_EQUAL(name, g_array_index(compiled->steps, xpath_step_t, 0).name);

  xpath_compiled_free(compiled);
}

typedef struct {
  document_t *doc;
  node_t *parent;
  node_t *e;
  node_t *tn;
} xpath_test_data_t;

static void
init_xpath_test(xpath_test_data_t *d)
{
  d->doc = document_new();
  d->parent = test_helper_new_node(d->doc, "parent");
  d->e = test_helper_new_node(d->doc, "child1");
  d->tn = test_helper_new_text_node(d->doc, "child2");

  node_append_child(d->parent, d->e);
  node_append_child(d->parent, d->tn);
}

static void
destroy_xpath_test(xpath_test_data_t *d)
{
  node_free(d->parent);
  document_free(d->doc);
}

static void
init_step(xpath_step_t *step)
{
  step->axis = XPATH_AXIS_CHILD;
  step->type = XPATH_NODE_TYPE_ELEMENT;
  step->name = NULL;
  step->predicates = NULL;
}

TEST(xpath, element)
{
  xpath_test_data_t d;
  nodeset_t *ns;
  const node_t *n;
  xpath_step_t step;

  init_xpath_test(&d);
  init_step(&step);

  ns = xpath_select_xpath_no_predicates(d.parent, &step);
  CHECK_EQUAL(1, nodeset_count(ns));
  n = nodeset_get(ns, 0);
  POINTERS_EQUAL(d.e, n);

  nodeset_free(ns);
  destroy_xpath_test(&d);
}

TEST(xpath, text_node)
{
  xpath_test_data_t d;
  nodeset_t *ns;
  const node_t *n;
  xpath_step_t step;

  init_xpath_test(&d);
  init_step(&step);
  step.type = XPATH_NODE_TYPE_TEXT_NODE;

  ns = xpath_select_xpath_no_predicates(d.parent, &step);
  CHECK_EQUAL(1, nodeset_count(ns));
  n = nodeset_get(ns, 0);
  POINTERS_EQUAL(d.tn, n);

  nodeset_free(ns);
  destroy_xpath_test(&d);
}

TEST(xpath, element_and_text_node)
{
  xpath_test_data_t d;
  nodeset_t *ns;
  const node_t *n;
  xpath_step_t step;

  init_xpath_test(&d);
  init_step(&step);
  step.type = XPATH_NODE_TYPE_ELEMENT | XPATH_NODE_TYPE_TEXT_NODE;

  ns = xpath_select_xpath_no_predicates(d.parent, &step);
  CHECK_EQUAL(2, nodeset_count(ns));
  n = nodeset_get(ns, 0);
  POINTERS_EQUAL(d.e, n);
  n = nodeset_get(ns, 1);
  POINTERS_EQUAL(d.tn, n);

  nodeset_free(ns);
  destroy_xpath_test(&d);
}

typedef struct {
  document_t *doc;
  node_t *alpha;
  node_t *one;
  node_t *a;
  node_t *b;
  node_t *c;
  node_t *d;
  node_t *two;
  node_t *w;
  node_t *x;
  node_t *y;
  node_t *z;
} xpath_axis_test_t;

static void
init_xpath_axis_test(xpath_axis_test_t *d)
{
  d->doc = document_new();
  d->alpha = test_helper_new_node(d->doc, "alpha");
  d->one = test_helper_new_node(d->doc, "one");
  d->a = test_helper_new_node(d->doc, "a");
  d->b = test_helper_new_node(d->doc, "b");
  d->c = test_helper_new_node(d->doc, "c");
  d->d = test_helper_new_node(d->doc, "d");
  d->two = test_helper_new_node(d->doc, "two");
  d->w = test_helper_new_node(d->doc, "w");
  d->x = test_helper_new_node(d->doc, "x");
  d->y = test_helper_new_node(d->doc, "y");
  d->z = test_helper_new_node(d->doc, "z");

  node_append_child(d->alpha, d->one);
  node_append_child(d->one, d->a);
  node_append_child(d->one, d->b);
  node_append_child(d->one, d->c);
  node_append_child(d->one, d->d);
  node_append_child(d->alpha, d->two);
  node_append_child(d->two, d->w);
  node_append_child(d->two, d->x);
  node_append_child(d->two, d->y);
  node_append_child(d->two, d->z);
}

static void
destroy_xpath_axis_test(xpath_axis_test_t *d)
{
  node_free(d->alpha);
  document_free(d->doc);
}

#define CHECK_nodeset_item(_node, _nodeset, _index) \
  POINTERS_EQUAL(_node, nodeset_get(_nodeset, _index))

TEST_GROUP(xpath_axis)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  void setup(void)
  {
    init_xpath_axis_test(&d);
    init_step(&step);
  }

  void teardown(void)
  {
    nodeset_free(ns);
    destroy_xpath_axis_test(&d);
  }
};

TEST(xpath_axis, axis_self)
{
  step.axis = XPATH_AXIS_SELF;

  ns = xpath_select_xpath_no_predicates(d.b, &step);
  CHECK_EQUAL(1, nodeset_count(ns));
  CHECK_nodeset_item(d.b, ns, 0);
}

TEST(xpath_axis, axis_parent)
{
  step.axis = XPATH_AXIS_PARENT;

  ns = xpath_select_xpath_no_predicates(d.b, &step);
  CHECK_EQUAL(1, nodeset_count(ns));
  CHECK_nodeset_item(d.one, ns, 0);
}

TEST(xpath_axis, axis_following_sibling)
{
  step.axis = XPATH_AXIS_FOLLOWING_SIBLING;

  ns = xpath_select_xpath_no_predicates(d.b, &step);
  CHECK_EQUAL(2, nodeset_count(ns));
  CHECK_nodeset_item(d.c, ns, 0);
  CHECK_nodeset_item(d.d, ns, 1);
}

TEST(xpath_axis, axis_preceding_sibling)
{
  step.axis = XPATH_AXIS_PRECEDING_SIBLING;

  ns = xpath_select_xpath_no_predicates(d.d, &step);
  CHECK_EQUAL(3, nodeset_count(ns));
  CHECK_nodeset_item(d.c, ns, 0);
  CHECK_nodeset_item(d.b, ns, 1);
  CHECK_nodeset_item(d.a, ns, 2);
}

TEST(xpath_axis, axis_descendant)
{
  step.axis = XPATH_AXIS_DESCENDANT;

  ns = xpath_select_xpath_no_predicates(d.alpha, &step);
  CHECK_EQUAL(10, nodeset_count(ns));
  CHECK_nodeset_item(d.one, ns, 0);
  CHECK_nodeset_item(d.a, ns, 1);
  CHECK_nodeset_item(d.b, ns, 2);
  CHECK_nodeset_item(d.c, ns, 3);
  CHECK_nodeset_item(d.d, ns, 4);
  CHECK_nodeset_item(d.two, ns, 5);
  CHECK_nodeset_item(d.w, ns, 6);
  CHECK_nodeset_item(d.x, ns, 7);
  CHECK_nodeset_item(d.y, ns, 8);
  CHECK_nodeset_item(d.z, ns, 9);
}

TEST(xpath_axis, axis_descendant_or_self)
{
  step.axis = XPATH_AXIS_DESCENDANT_OR_SELF;

  ns = xpath_select_xpath_no_predicates(d.alpha, &step);
  CHECK_EQUAL(11, nodeset_count(ns));
  CHECK_nodeset_item(d.alpha, ns, 0);
  CHECK_nodeset_item(d.one, ns, 1);
  CHECK_nodeset_item(d.a, ns, 2);
  CHECK_nodeset_item(d.b, ns, 3);
  CHECK_nodeset_item(d.c, ns, 4);
  CHECK_nodeset_item(d.d, ns, 5);
  CHECK_nodeset_item(d.two, ns, 6);
  CHECK_nodeset_item(d.w, ns, 7);
  CHECK_nodeset_item(d.x, ns, 8);
  CHECK_nodeset_item(d.y, ns, 9);
  CHECK_nodeset_item(d.z, ns, 10);
}

TEST(xpath_axis, axis_ancestor)
{
  step.axis = XPATH_AXIS_ANCESTOR;

  ns = xpath_select_xpath_no_predicates(d.b, &step);
  CHECK_EQUAL(2, nodeset_count(ns));
  CHECK_nodeset_item(d.one, ns, 0);
  CHECK_nodeset_item(d.alpha, ns, 1);
}

TEST(xpath_axis, axis_ancestor_or_self)
{
  step.axis = XPATH_AXIS_ANCESTOR_OR_SELF;

  ns = xpath_select_xpath_no_predicates(d.c, &step);
  CHECK_EQUAL(3, nodeset_count(ns));
  CHECK_nodeset_item(d.c, ns, 0);
  CHECK_nodeset_item(d.one, ns, 1);
  CHECK_nodeset_item(d.alpha, ns, 2);
}

TEST(xpath_axis, axis_following)
{
  step.axis = XPATH_AXIS_FOLLOWING;

  ns = xpath_select_xpath_no_predicates(d.c, &step);
  CHECK_EQUAL(6, nodeset_count(ns));
  CHECK_nodeset_item(d.d, ns, 0);
  CHECK_nodeset_item(d.two, ns, 1);
  CHECK_nodeset_item(d.w, ns, 2);
  CHECK_nodeset_item(d.x, ns, 3);
  CHECK_nodeset_item(d.y, ns, 4);
  CHECK_nodeset_item(d.z, ns, 5);
}

TEST(xpath_axis, axis_preceding)
{
  step.axis = XPATH_AXIS_PRECEDING;

  ns = xpath_select_xpath_no_predicates(d.x, &step);
  CHECK_EQUAL(6, nodeset_count(ns));
  CHECK_nodeset_item(d.w, ns, 0);
  CHECK_nodeset_item(d.one, ns, 1);
  CHECK_nodeset_item(d.a, ns, 2);
  CHECK_nodeset_item(d.b, ns, 3);
  CHECK_nodeset_item(d.c, ns, 4);
  CHECK_nodeset_item(d.d, ns, 5);
}

TEST(xpath, two_step)
{
  nodeset_t *ns;
  xpath_step_t step;
  GArray *steps;
  xpath_axis_test_t d;

  init_xpath_axis_test(&d);
  steps = g_array_new(FALSE, FALSE, sizeof(xpath_step_t));

  init_step(&step);
  step.name = strdup("one");
  g_array_append_val(steps, step);
  step.name = strdup("c");
  g_array_append_val(steps, step);

  ns = xpath_select_xpath_steps(d.alpha, steps);
  CHECK_EQUAL(1, nodeset_count(ns));
  CHECK_nodeset_item(d.c, ns, 0);

  g_array_free(steps, TRUE);
  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
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

/* 4.1 - Node set functions */

TEST(xpath, fn_last)
{
  xpath_axis_test_t d;
  nodeset_t *ns;
  xpath_result_t res;
  xpath_evaluation_context_t context;

  init_xpath_axis_test(&d);

  ns = nodeset_new_with_nodes(d.a, d.b, d.c, NULL);

  context.nodeset = ns;

  context.node = d.a;
  res = xpath_fn_last(&context, NULL);
  CHECK_RESULT_NUMERIC(res, 3);

  context.node = d.b;
  res = xpath_fn_last(&context, NULL);
  CHECK_RESULT_NUMERIC(res, 3);

  context.node = d.c;
  res = xpath_fn_last(&context, NULL);
  CHECK_RESULT_NUMERIC(res, 3);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

TEST(xpath, fn_position)
{
  xpath_axis_test_t d;
  nodeset_t *ns;
  xpath_result_t res;
  xpath_evaluation_context_t context;

  init_xpath_axis_test(&d);

  ns = nodeset_new_with_nodes(d.a, d.b, d.c, NULL);

  context.nodeset = ns;

  context.node = d.a;
  res = xpath_fn_position(&context, NULL);
  CHECK_RESULT_NUMERIC(res, 1);

  context.node = d.b;
  res = xpath_fn_position(&context, NULL);
  CHECK_RESULT_NUMERIC(res, 2);

  context.node = d.c;
  res = xpath_fn_position(&context, NULL);
  CHECK_RESULT_NUMERIC(res, 3);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

/* 4.2 - String functions */

TEST(xpath, fn_concat_2)
{
  GArray *parameters;
  xpath_result_t res;

  parameters = string_parameters("one", "two", NULL);

  res = xpath_fn_concat(NULL, parameters);
  CHECK_RESULT_STRING(res, "onetwo");

  g_array_free(parameters, TRUE);
}

TEST(xpath, fn_concat_3)
{
  GArray *parameters;
  xpath_result_t res;

  parameters = string_parameters("one", "two", "three", NULL);

  res = xpath_fn_concat(NULL, parameters);
  CHECK_RESULT_STRING(res, "onetwothree");

  g_array_free(parameters, TRUE);
}

TEST(xpath, fn_starts_with)
{
  GArray *parameters;
  xpath_result_t res;

  parameters = string_parameters("hello world", "hello", NULL);

  res = xpath_fn_starts_with(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);

  g_array_free(parameters, TRUE);
}

TEST(xpath, fn_starts_with_failure)
{
  GArray *parameters;
  xpath_result_t res;

  parameters = string_parameters("hello world", "cow");
  res = xpath_fn_starts_with(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);

  g_array_free(parameters, TRUE);
}

TEST(xpath, fn_contains)
{
  GArray *parameters;
  xpath_result_t res;

  parameters = string_parameters("hello world", "world", NULL);
  res = xpath_fn_contains(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, TRUE);

  g_array_free(parameters, TRUE);
}

TEST(xpath, fn_contains_failure)
{
  GArray *parameters;
  xpath_result_t res;

  parameters = string_parameters("hello world", "cow", NULL);

  res = xpath_fn_contains(NULL, parameters);
  CHECK_RESULT_BOOLEAN(res, FALSE);

  g_array_free(parameters, TRUE);
}

TEST(xpath, fn_substring_before)
{
  GArray *parameters;
  xpath_result_t res;

  parameters = string_parameters("1999/04/01", "/", NULL);

  res = xpath_fn_substring_before(NULL, parameters);
  CHECK_RESULT_STRING(res, "1999");

  g_array_free(parameters, TRUE);
}

TEST(xpath, fn_substring_before_failure)
{
  GArray *parameters;
  xpath_result_t res;

  parameters = string_parameters("hello world", "cow", NULL);

  res = xpath_fn_substring_before(NULL, parameters);
  CHECK_RESULT_STRING(res, "");

  g_array_free(parameters, TRUE);
}

TEST(xpath, fn_substring_after)
{
  GArray *parameters;
  xpath_result_t res;

  parameters = string_parameters("1999/04/01", "/", NULL);

  res = xpath_fn_substring_after(NULL, parameters);
  CHECK_RESULT_STRING(res, "04/01");

  g_array_free(parameters, TRUE);
}

TEST(xpath, fn_substring_after_failure)
{
  GArray *parameters;
  xpath_result_t res;

  parameters = string_parameters("hello world", "cow", NULL);

  res = xpath_fn_substring_after(NULL, parameters);
  CHECK_RESULT_STRING(res, "");

  g_array_free(parameters, TRUE);
}

TEST(xpath, fn_substring_2)
{
  GArray *parameters;
  xpath_result_t res;

  parameters = string_parameters("12345", NULL);
  add_number_param(parameters, 2);

  res = xpath_fn_substring(NULL, parameters);
  CHECK_RESULT_STRING(res, "2345");

  g_array_free(parameters, TRUE);
}

TEST(xpath, fn_substring_3)
{
  GArray *parameters;
  xpath_result_t res;

  parameters = string_parameters("12345", NULL);
  add_number_param(add_number_param(parameters, 2), 3);

  res = xpath_fn_substring(NULL, parameters);
  CHECK_RESULT_STRING(res, "234");

  g_array_free(parameters, TRUE);
}

TEST(xpath, fn_substring_unicode)
{
  GArray *parameters;
  xpath_result_t res;

  parameters = string_parameters("ウィキペディア", NULL);
  add_number_param(add_number_param(parameters, 2), 3);

  res = xpath_fn_substring(NULL, parameters);
  CHECK_RESULT_STRING(res, "ィキペ");

  g_array_free(parameters, TRUE);
}

/* 4.3 - Boolean functions */

TEST(xpath, fn_true)
{
  xpath_result_t res;
  res = xpath_fn_true(NULL, NULL);
  CHECK_RESULT_BOOLEAN(res, TRUE);
}

TEST(xpath, fn_false)
{
  xpath_result_t res;
  res = xpath_fn_false(NULL, NULL);
  CHECK_RESULT_BOOLEAN(res, FALSE);
}

/* 4.4 - Number functions */

TEST(xpath, fn_floor)
{
  GArray *parameters;
  xpath_result_t *value;
  xpath_result_t res;

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

  g_array_free(parameters, TRUE);
}

TEST(xpath, fn_ceiling)
{
  GArray *parameters;
  xpath_result_t *value;
  xpath_result_t res;

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

  g_array_free(parameters, TRUE);
}

TEST(xpath, fn_round)
{
  GArray *parameters;
  xpath_result_t *value;
  xpath_result_t res;

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

  g_array_free(parameters, TRUE);
}

TEST_GROUP(xpath_predicate)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  void setup(void)
  {
    init_xpath_axis_test(&d);
    init_step(&step);
  }

  void teardown(void)
  {
    g_list_free(step.predicates);
    nodeset_free(ns);
    destroy_xpath_axis_test(&d);
  }
};

TEST(xpath_predicate, predicate_true)
{
  step.predicates = g_list_append(step.predicates, &g_predicate_value_true);

  ns = nodeset_new_with_nodes(d.alpha, NULL);
  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(1, nodeset_count(ns));
}

TEST(xpath_predicate, predicate_false)
{
  step.predicates = g_list_append(step.predicates, &g_predicate_value_false);

  ns = nodeset_new_with_nodes(d.alpha, NULL);
  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(0, nodeset_count(ns));
}

TEST(xpath_predicate, predicate_value_3)
{
  xpath_predicate_t pred_val_3;

  pred_val_3.op = XPATH_PREDICATE_OP_VALUE;
  pred_val_3.info.value.type = XPATH_RESULT_TYPE_NUMERIC;
  pred_val_3.info.value.value.numeric = 3;
  step.predicates = g_list_append(step.predicates, &pred_val_3);

  ns = nodeset_new_with_nodes(d.a, d.b, d.c, d.d, NULL);

  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(1, nodeset_count(ns));
  CHECK_nodeset_item(d.c, ns, 0);
}

TEST(xpath_predicate, predicate_fn_true)
{
  xpath_predicate_t pred;

  pred.op = XPATH_PREDICATE_OP_FUNCTION;
  pred.info.function.fn = xpath_fn_true;
  pred.info.function.parameters = NULL;
  step.predicates = g_list_append(step.predicates, &pred);

  ns = nodeset_new_with_nodes(d.alpha, NULL);
  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(1, nodeset_count(ns));
}

TEST(xpath_predicate, predicate_fn_false)
{
  xpath_predicate_t pred;

  pred.op = XPATH_PREDICATE_OP_FUNCTION;
  pred.info.function.fn = xpath_fn_false;
  pred.info.function.parameters = NULL;
  step.predicates = g_list_append(step.predicates, &pred);

  ns = nodeset_new_with_nodes(d.alpha, NULL);
  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(0, nodeset_count(ns));
}

TEST(xpath_predicate, predicate_fn_floor)
{
  xpath_predicate_t pred;
  xpath_result_t value;
  GArray *parameters;

  init_xpath_axis_test(&d);
  init_step(&step);
  parameters = g_array_new(FALSE, FALSE, sizeof(xpath_result_t));
  value.type = XPATH_RESULT_TYPE_NUMERIC;
  value.value.numeric = 1.3;
  g_array_append_val(parameters, value);

  pred.op = XPATH_PREDICATE_OP_FUNCTION;
  pred.info.function.fn = xpath_fn_floor;
  pred.info.function.parameters = parameters;
  step.predicates = g_list_append(step.predicates, &pred);

  ns = nodeset_new_with_nodes(d.alpha, NULL);
  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(1, nodeset_count(ns));
}

TEST(xpath_predicate, predicate_fn_ceiling)
{
  xpath_predicate_t pred;
  xpath_result_t value;
  GArray *parameters;

  parameters = g_array_new(FALSE, FALSE, sizeof(xpath_result_t));
  value.type = XPATH_RESULT_TYPE_NUMERIC;
  value.value.numeric = 0.1;
  g_array_append_val(parameters, value);

  pred.op = XPATH_PREDICATE_OP_FUNCTION;
  pred.info.function.fn = xpath_fn_ceiling;
  pred.info.function.parameters = parameters;
  step.predicates = g_list_append(step.predicates, &pred);

  ns = nodeset_new_with_nodes(d.alpha, NULL);
  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(1, nodeset_count(ns));
}

TEST(xpath_predicate, predicate_equal_true)
{
  xpath_predicate_t pred;

  pred.op = XPATH_PREDICATE_OP_EQUAL;
  pred.info.child.left = &g_predicate_value_true;
  pred.info.child.right = &g_predicate_value_true;
  step.predicates = g_list_append(step.predicates, &pred);

  ns = nodeset_new_with_nodes(d.alpha, NULL);
  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(1, nodeset_count(ns));
}

TEST(xpath_predicate, predicate_equal_false)
{
  xpath_predicate_t pred;

  pred.op = XPATH_PREDICATE_OP_EQUAL;
  pred.info.child.left = &g_predicate_value_true;
  pred.info.child.right = &g_predicate_value_false;
  step.predicates = g_list_append(step.predicates, &pred);

  ns = nodeset_new_with_nodes(d.alpha, NULL);
  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(0, nodeset_count(ns));
}

TEST(xpath_predicate, predicate_position_1)
{
  xpath_predicate_t pred;
  xpath_predicate_t pred_val_1;
  xpath_predicate_t pred_fn_position;

  pred_val_1.op = XPATH_PREDICATE_OP_VALUE;
  pred_val_1.info.value.type = XPATH_RESULT_TYPE_NUMERIC;
  pred_val_1.info.value.value.numeric = 1;

  pred_fn_position.op = XPATH_PREDICATE_OP_FUNCTION;
  pred_fn_position.info.function.fn = xpath_fn_position;
  pred_fn_position.info.function.parameters = NULL;

  pred.op = XPATH_PREDICATE_OP_EQUAL;
  pred.info.child.left = &pred_val_1;
  pred.info.child.right = &pred_fn_position;
  step.predicates = g_list_append(step.predicates, &pred);

  ns = nodeset_new_with_nodes(d.alpha, NULL);
  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(1, nodeset_count(ns));
}

#define CHECK_nodeset_element_name(_nodeset, _index, _name) \
  {							     \
    element_t *__e;					     \
    node_t *__n;					     \
    __n = nodeset_get(_nodeset, _index);		     \
    CHECK_EQUAL(NODE_TYPE_ELEMENT, node_type(__n));		     \
    __e = (element_t *)__n;				     \
    STRCMP_EQUAL(_name, element_name(__e));		     \
  }

TEST(xpath, apply_element)
{
  const char * const name = "one";

  document_t *doc = document_new();
  node_t *parent = test_helper_new_node(doc, "parent");
  node_t *children[4];
  nodeset_t *nodes;

  children[0] = test_helper_new_node(doc, "one");
  children[1] = test_helper_new_node(doc, "two");
  children[2] = test_helper_new_node(doc, "one");
  children[3] = test_helper_new_node(doc, "four");

  node_append_child(parent, children[0]);
  node_append_child(parent, children[1]);
  node_append_child(parent, children[2]);
  node_append_child(parent, children[3]);

  nodes = xpath_apply_xpath(parent, name);

  CHECK_EQUAL(2, nodeset_count(nodes));
  CHECK_nodeset_element_name(nodes, 0, name);
  CHECK_nodeset_element_name(nodes, 1, name);

  nodeset_free(nodes);
  node_free(parent);
  document_free(doc);
}

static void
initialize_globals(void)
{
  g_predicate_value_true.op = XPATH_PREDICATE_OP_VALUE;
  g_predicate_value_true.info.value.type = XPATH_RESULT_TYPE_BOOLEAN;
  g_predicate_value_true.info.value.value.boolean = TRUE;

  g_predicate_value_false.op = XPATH_PREDICATE_OP_VALUE;
  g_predicate_value_false.info.value.type = XPATH_RESULT_TYPE_BOOLEAN;
  g_predicate_value_false.info.value.value.boolean = FALSE;
}

int
main(int argc, char **argv)
{
  initialize_globals();
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
