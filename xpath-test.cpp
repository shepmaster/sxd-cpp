#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "xpath-internal.h"
#include "xpath-functions.h"
#include "test-utilities.h"

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

static void
destroy_step(xpath_step_t *step)
{
  if (step->name) free(step->name);
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
  unsigned int i;

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

  for (i = 0; i < steps->len; i++) {
    xpath_step_t *x;
    x = &g_array_index(steps, xpath_step_t, i);
    destroy_step(x);
  }
  g_array_free(steps, TRUE);
  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
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

  g_array_free(parameters, TRUE);
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

  g_array_free(parameters, TRUE);
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
