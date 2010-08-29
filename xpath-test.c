#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "xpath-internal.h"
#include "test-utilities.h"

static xpath_predicate_t g_predicate_value_true;
static xpath_predicate_t g_predicate_value_false;

void
dump_xpath_tokens(xpath_tokens_t *tokens)
{
  int i;
  for (i = 0; i < tokens->tokens->len; i++) {
    xpath_token_t *t = &g_array_index(tokens->tokens, xpath_token_t, i);
    printf("%d\n", t->start);
  }
}

#define assert_token(_tokens, _index, _type, _start)			\
  {									\
    xpath_token_t *__token;						\
    __token = &g_array_index(_tokens->tokens, xpath_token_t, _index);	\
    assert(_type == __token->type);					\
    assert(_start == __token->start);					\
}

static void
test_xpath_tokenize(void)
{
  xpath_tokens_t *tokens;
  const char * const xpath = "//one/two";
  tokens = xpath_tokenize(xpath);

  assert(strcmp(xpath, tokens->xpath) == 0);
  assert(5 == tokens->tokens->len);

  assert_token(tokens, 0, SLASH, 0);
  assert_token(tokens, 1, SLASH, 1);
  assert_token(tokens, 2, TEXT,  2);
  assert_token(tokens, 3, SLASH, 5);
  assert_token(tokens, 4, TEXT,  6);

  xpath_tokens_free(tokens);
}

#define assert_str(_tokens, _index, _str)		\
  {							\
    char *__str = xpath_tokens_string(_tokens, _index);	\
    assert(strcmp(__str, _str) == 0);			\
    free(__str);					\
  }							\

static void
test_xpath_tokens_string(void)
{
  xpath_tokens_t *tokens;
  const char * const xpath = "//one/two";

  tokens = xpath_tokenize(xpath);
  assert_str(tokens, 0, "/");
  assert_str(tokens, 1, "/");
  assert_str(tokens, 2, "one");
  assert_str(tokens, 3, "/");
  assert_str(tokens, 4, "two");

  xpath_tokens_free(tokens);
}

static void
test_xpath_compile_element(void)
{
  const char * const name = "one";
  xpath_compiled_t *compiled;

  compiled = xpath_compile(name);

  assert(1 == compiled->steps->len);
  assert(XPATH_AXIS_CHILD == g_array_index(compiled->steps, xpath_step_t, 0).axis);
  assert(XPATH_NODE_TYPE_ELEMENT == g_array_index(compiled->steps, xpath_step_t, 0).type);
  assert(strcmp(name, g_array_index(compiled->steps, xpath_step_t, 0).name) == 0);
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
test_xpath_element(void)
{
  xpath_test_data_t d;
  nodeset_t *ns;
  const node_t *n;
  xpath_step_t step;

  init_xpath_test(&d);
  init_step(&step);

  ns = xpath_select_xpath_no_predicates(d.parent, &step);
  assert(1 == nodeset_count(ns));
  n = nodeset_get(ns, 0);
  assert(n == d.e);

  nodeset_free(ns);
  destroy_xpath_test(&d);
}

static void
test_xpath_text_node(void)
{
  xpath_test_data_t d;
  nodeset_t *ns;
  const node_t *n;
  xpath_step_t step;

  init_xpath_test(&d);
  init_step(&step);
  step.type = XPATH_NODE_TYPE_TEXT_NODE;

  ns = xpath_select_xpath_no_predicates(d.parent, &step);
  assert(1 == nodeset_count(ns));
  n = nodeset_get(ns, 0);
  assert(n == d.tn);

  nodeset_free(ns);
  destroy_xpath_test(&d);
}

static void
test_xpath_element_and_text_node(void)
{
  xpath_test_data_t d;
  nodeset_t *ns;
  const node_t *n;
  xpath_step_t step;

  init_xpath_test(&d);
  init_step(&step);
  step.type = XPATH_NODE_TYPE_ELEMENT | XPATH_NODE_TYPE_TEXT_NODE;

  ns = xpath_select_xpath_no_predicates(d.parent, &step);
  assert(2 == nodeset_count(ns));
  n = nodeset_get(ns, 0);
  assert(n == d.e);
  n = nodeset_get(ns, 1);
  assert(n == d.tn);

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

#define assert_nodeset_item(_node, _nodeset, _index) \
  assert(_node == nodeset_get(_nodeset, _index))

static void
test_xpath_axis_self(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  init_xpath_axis_test(&d);
  init_step(&step);
  step.axis = XPATH_AXIS_SELF;

  ns = xpath_select_xpath_no_predicates(d.b, &step);
  assert(1 == nodeset_count(ns));
  assert_nodeset_item(d.b, ns, 0);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_axis_parent(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  init_xpath_axis_test(&d);
  init_step(&step);
  step.axis = XPATH_AXIS_PARENT;

  ns = xpath_select_xpath_no_predicates(d.b, &step);
  assert(1 == nodeset_count(ns));
  assert_nodeset_item(d.one, ns, 0);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_axis_following_sibling(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  init_xpath_axis_test(&d);
  init_step(&step);
  step.axis = XPATH_AXIS_FOLLOWING_SIBLING;

  ns = xpath_select_xpath_no_predicates(d.b, &step);
  assert(2 == nodeset_count(ns));
  assert_nodeset_item(d.c, ns, 0);
  assert_nodeset_item(d.d, ns, 1);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_axis_preceding_sibling(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  init_xpath_axis_test(&d);
  init_step(&step);
  step.axis = XPATH_AXIS_PRECEDING_SIBLING;

  ns = xpath_select_xpath_no_predicates(d.d, &step);
  assert(3 == nodeset_count(ns));
  assert_nodeset_item(d.c, ns, 0);
  assert_nodeset_item(d.b, ns, 1);
  assert_nodeset_item(d.a, ns, 2);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_axis_descendant(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  init_xpath_axis_test(&d);
  init_step(&step);
  step.axis = XPATH_AXIS_DESCENDANT;

  ns = xpath_select_xpath_no_predicates(d.alpha, &step);
  assert(10 == nodeset_count(ns));
  assert_nodeset_item(d.one, ns, 0);
  assert_nodeset_item(d.a, ns, 1);
  assert_nodeset_item(d.b, ns, 2);
  assert_nodeset_item(d.c, ns, 3);
  assert_nodeset_item(d.d, ns, 4);
  assert_nodeset_item(d.two, ns, 5);
  assert_nodeset_item(d.w, ns, 6);
  assert_nodeset_item(d.x, ns, 7);
  assert_nodeset_item(d.y, ns, 8);
  assert_nodeset_item(d.z, ns, 9);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_axis_descendant_or_self(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  init_xpath_axis_test(&d);
  init_step(&step);
  step.axis = XPATH_AXIS_DESCENDANT_OR_SELF;

  ns = xpath_select_xpath_no_predicates(d.alpha, &step);
  assert(11 == nodeset_count(ns));
  assert_nodeset_item(d.alpha, ns, 0);
  assert_nodeset_item(d.one, ns, 1);
  assert_nodeset_item(d.a, ns, 2);
  assert_nodeset_item(d.b, ns, 3);
  assert_nodeset_item(d.c, ns, 4);
  assert_nodeset_item(d.d, ns, 5);
  assert_nodeset_item(d.two, ns, 6);
  assert_nodeset_item(d.w, ns, 7);
  assert_nodeset_item(d.x, ns, 8);
  assert_nodeset_item(d.y, ns, 9);
  assert_nodeset_item(d.z, ns, 10);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_axis_ancestor(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  init_xpath_axis_test(&d);
  init_step(&step);
  step.axis = XPATH_AXIS_ANCESTOR;

  ns = xpath_select_xpath_no_predicates(d.b, &step);
  assert(2 == nodeset_count(ns));
  assert_nodeset_item(d.one, ns, 0);
  assert_nodeset_item(d.alpha, ns, 1);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_axis_ancestor_or_self(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  init_xpath_axis_test(&d);
  init_step(&step);
  step.axis = XPATH_AXIS_ANCESTOR_OR_SELF;

  ns = xpath_select_xpath_no_predicates(d.c, &step);
  assert(3 == nodeset_count(ns));
  assert_nodeset_item(d.c, ns, 0);
  assert_nodeset_item(d.one, ns, 1);
  assert_nodeset_item(d.alpha, ns, 2);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_axis_following(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  init_xpath_axis_test(&d);
  init_step(&step);
  step.axis = XPATH_AXIS_FOLLOWING;

  ns = xpath_select_xpath_no_predicates(d.c, &step);
  assert(6 == nodeset_count(ns));
  assert_nodeset_item(d.d, ns, 0);
  assert_nodeset_item(d.two, ns, 1);
  assert_nodeset_item(d.w, ns, 2);
  assert_nodeset_item(d.x, ns, 3);
  assert_nodeset_item(d.y, ns, 4);
  assert_nodeset_item(d.z, ns, 5);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_axis_preceding(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  init_xpath_axis_test(&d);
  init_step(&step);
  step.axis = XPATH_AXIS_PRECEDING;

  ns = xpath_select_xpath_no_predicates(d.x, &step);
  assert(6 == nodeset_count(ns));
  assert_nodeset_item(d.w, ns, 0);
  assert_nodeset_item(d.one, ns, 1);
  assert_nodeset_item(d.a, ns, 2);
  assert_nodeset_item(d.b, ns, 3);
  assert_nodeset_item(d.c, ns, 4);
  assert_nodeset_item(d.d, ns, 5);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_two_step(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  GArray *steps;
  xpath_axis_test_t d;

  init_xpath_axis_test(&d);
  steps = g_array_new(FALSE, FALSE, sizeof(xpath_step_t));

  init_step(&step);
  step.name = "one";
  g_array_append_val(steps, step);
  step.name = "c";
  g_array_append_val(steps, step);

  ns = xpath_select_xpath_steps(d.alpha, steps);
  assert(1 == nodeset_count(ns));
  assert_nodeset_item(d.c, ns, 0);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_fn_true(void)
{
  xpath_result_t res;
  res = xpath_fn_true(NULL);
  assert(res.type == XPATH_RESULT_TYPE_BOOLEAN);
  assert(res.boolean == TRUE);
}

static void
test_xpath_fn_false(void)
{
  xpath_result_t res;
  res = xpath_fn_false(NULL);
  assert(res.type == XPATH_RESULT_TYPE_BOOLEAN);
  assert(res.boolean == FALSE);
}

static void
test_xpath_fn_position(void)
{
  xpath_axis_test_t d;
  nodeset_t *ns;
  xpath_result_t res;
  xpath_evaluation_context_t context;

  init_xpath_axis_test(&d);

  ns = nodeset_new();
  nodeset_add(ns, d.a);
  nodeset_add(ns, d.b);
  nodeset_add(ns, d.c);

  context.nodeset = ns;

  context.node = d.a;
  res = xpath_fn_position(&context);
  assert(res.type == XPATH_RESULT_TYPE_INTEGER);
  assert(res.integer == 1);

  context.node = d.b;
  res = xpath_fn_position(&context);
  assert(res.type == XPATH_RESULT_TYPE_INTEGER);
  assert(res.integer == 2);

  context.node = d.c;
  res = xpath_fn_position(&context);
  assert(res.type == XPATH_RESULT_TYPE_INTEGER);
  assert(res.integer == 3);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_predicate_true(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  init_xpath_axis_test(&d);
  init_step(&step);

  step.predicates = g_list_append(step.predicates, &g_predicate_value_true);

  ns = nodeset_new();
  nodeset_add(ns, d.alpha);
  ns = xpath_apply_predicates(ns, &step);
  assert(1 == nodeset_count(ns));

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_predicate_false(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  init_xpath_axis_test(&d);
  init_step(&step);

  step.predicates = g_list_append(step.predicates, &g_predicate_value_false);

  ns = nodeset_new();
  nodeset_add(ns, d.alpha);
  ns = xpath_apply_predicates(ns, &step);
  assert(0 == nodeset_count(ns));

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_predicate_value_3(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;
  xpath_predicate_t pred_val_3;

  init_xpath_axis_test(&d);
  init_step(&step);

  pred_val_3.op = XPATH_PREDICATE_OP_VALUE;
  pred_val_3.info.value.type = XPATH_RESULT_TYPE_INTEGER;
  pred_val_3.info.value.integer = 3;
  step.predicates = g_list_append(step.predicates, &pred_val_3);

  ns = nodeset_new();
  nodeset_add(ns, d.a);
  nodeset_add(ns, d.b);
  nodeset_add(ns, d.c);
  nodeset_add(ns, d.d);

  ns = xpath_apply_predicates(ns, &step);
  assert(1 == nodeset_count(ns));
  assert_nodeset_item(d.c, ns, 0);

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_predicate_fn_true(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;
  xpath_predicate_t pred;

  init_xpath_axis_test(&d);
  init_step(&step);

  pred.op = XPATH_PREDICATE_OP_FUNCTION;
  pred.info.fn = xpath_fn_true;
  step.predicates = g_list_append(step.predicates, &pred);

  ns = nodeset_new();
  nodeset_add(ns, d.alpha);
  ns = xpath_apply_predicates(ns, &step);
  assert(1 == nodeset_count(ns));

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_predicate_fn_false(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;
  xpath_predicate_t pred;

  init_xpath_axis_test(&d);
  init_step(&step);

  pred.op = XPATH_PREDICATE_OP_FUNCTION;
  pred.info.fn = xpath_fn_false;
  step.predicates = g_list_append(step.predicates, &pred);

  ns = nodeset_new();
  nodeset_add(ns, d.alpha);
  ns = xpath_apply_predicates(ns, &step);
  assert(0 == nodeset_count(ns));

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_predicate_equal_true(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;
  xpath_predicate_t pred;

  init_xpath_axis_test(&d);
  init_step(&step);

  pred.op = XPATH_PREDICATE_OP_EQUAL;
  pred.info.child.left = &g_predicate_value_true;
  pred.info.child.right = &g_predicate_value_true;
  step.predicates = g_list_append(step.predicates, &pred);

  ns = nodeset_new();
  nodeset_add(ns, d.alpha);
  ns = xpath_apply_predicates(ns, &step);
  assert(1 == nodeset_count(ns));

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_predicate_equal_false(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;
  xpath_predicate_t pred;

  init_xpath_axis_test(&d);
  init_step(&step);

  pred.op = XPATH_PREDICATE_OP_EQUAL;
  pred.info.child.left = &g_predicate_value_true;
  pred.info.child.right = &g_predicate_value_false;
  step.predicates = g_list_append(step.predicates, &pred);

  ns = nodeset_new();
  nodeset_add(ns, d.alpha);
  ns = xpath_apply_predicates(ns, &step);
  assert(0 == nodeset_count(ns));

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

static void
test_xpath_predicate_position_1(void)
{
  nodeset_t *ns;
  xpath_step_t step;
  xpath_axis_test_t d;
  xpath_predicate_t pred;
  xpath_predicate_t pred_val_1;
  xpath_predicate_t pred_fn_position;

  init_xpath_axis_test(&d);
  init_step(&step);

  pred_val_1.op = XPATH_PREDICATE_OP_VALUE;
  pred_val_1.info.value.type = XPATH_RESULT_TYPE_INTEGER;
  pred_val_1.info.value.integer = 1;

  pred_fn_position.op = XPATH_PREDICATE_OP_FUNCTION;
  pred_fn_position.info.fn = xpath_fn_position;

  pred.op = XPATH_PREDICATE_OP_EQUAL;
  pred.info.child.left = &pred_val_1;
  pred.info.child.right = &pred_fn_position;
  step.predicates = g_list_append(step.predicates, &pred);

  ns = nodeset_new();
  nodeset_add(ns, d.alpha);
  ns = xpath_apply_predicates(ns, &step);
  assert(1 == nodeset_count(ns));

  nodeset_free(ns);
  destroy_xpath_axis_test(&d);
}

#define assert_nodeset_element_name(_nodeset, _index, _name) \
  {							     \
    element_t *__e;					     \
    node_t *__n;					     \
    __n = nodeset_get(_nodeset, _index);		     \
    assert(node_type(__n) == NODE_TYPE_ELEMENT);	     \
    __e = (element_t *)__n;				     \
    assert(strcmp(element_name(__e), _name) == 0);	     \
  }

static void
test_xpath_apply_element(void)
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

  assert(2 == nodeset_count(nodes));
  assert_nodeset_element_name(nodes, 0, name);
  assert_nodeset_element_name(nodes, 1, name);

  nodeset_free(nodes);
  node_free(parent);
  document_free(doc);
}

static void
initialize_globals(void)
{
  g_predicate_value_true.op = XPATH_PREDICATE_OP_VALUE;
  g_predicate_value_true.info.value.type = XPATH_RESULT_TYPE_BOOLEAN;
  g_predicate_value_true.info.value.boolean = TRUE;

  g_predicate_value_false.op = XPATH_PREDICATE_OP_VALUE;
  g_predicate_value_false.info.value.type = XPATH_RESULT_TYPE_BOOLEAN;
  g_predicate_value_false.info.value.boolean = FALSE;
}

int
main(int argc, char **argv)
{
  initialize_globals();

  test_xpath_tokenize();
  test_xpath_tokens_string();
  test_xpath_compile_element();
  test_xpath_element();
  test_xpath_text_node();
  test_xpath_element_and_text_node();
  test_xpath_axis_self();
  test_xpath_axis_parent();
  test_xpath_axis_following_sibling();
  test_xpath_axis_preceding_sibling();
  test_xpath_axis_descendant();
  test_xpath_axis_descendant_or_self();
  test_xpath_axis_ancestor();
  test_xpath_axis_ancestor_or_self();
  test_xpath_axis_following();
  test_xpath_axis_preceding();
  test_xpath_two_step();
  test_xpath_apply_element();
  test_xpath_fn_true();
  test_xpath_fn_false();
  test_xpath_fn_position();
  test_xpath_predicate_true();
  test_xpath_predicate_false();
  test_xpath_predicate_value_3();
  test_xpath_predicate_fn_true();
  test_xpath_predicate_fn_false();
  test_xpath_predicate_equal_true();
  test_xpath_predicate_equal_false();
  test_xpath_predicate_position_1();

  return EXIT_SUCCESS;
}
