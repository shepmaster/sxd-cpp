#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "xpath-internal.h"
#include "test-utilities.h"

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
test_xpath_element(void)
{
  xpath_test_data_t d;
  nodeset_t *ns;
  const node_t *n;
  xpath_step_t step;

  init_xpath_test(&d);

  step.axis = XPATH_AXIS_CHILD;
  step.type = XPATH_NODE_TYPE_ELEMENT;
  step.name = NULL;

  ns = xpath_select_xpath(d.parent, &step);
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

  step.axis = XPATH_AXIS_CHILD;
  step.type = XPATH_NODE_TYPE_TEXT_NODE;
  step.name = NULL;

  ns = xpath_select_xpath(d.parent, &step);
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

  step.axis = XPATH_AXIS_CHILD;
  step.type = XPATH_NODE_TYPE_ELEMENT | XPATH_NODE_TYPE_TEXT_NODE;
  step.name = NULL;

  ns = xpath_select_xpath(d.parent, &step);
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
  node_t *one;
  node_t *a;
  node_t *b;
  node_t *c;
  node_t *d;
} xpath_sibling_test_t;

static void
init_xpath_sibling_test(xpath_sibling_test_t *d)
{
  d->doc = document_new();
  d->one = test_helper_new_node(d->doc, "one");
  d->a = test_helper_new_node(d->doc, "a");
  d->b = test_helper_new_node(d->doc, "b");
  d->c = test_helper_new_node(d->doc, "c");
  d->d = test_helper_new_node(d->doc, "d");

  node_append_child(d->one, d->a);
  node_append_child(d->one, d->b);
  node_append_child(d->one, d->c);
  node_append_child(d->one, d->d);
}

static void
destroy_xpath_sibling_test(xpath_sibling_test_t *d)
{
  node_free(d->one);
  document_free(d->doc);
}

static void
test_xpath_axis_self(void)
{
  nodeset_t *ns;
  const node_t *n;
  xpath_step_t step;
  xpath_sibling_test_t d;

  init_xpath_sibling_test(&d);

  step.axis = XPATH_AXIS_SELF;
  step.type = XPATH_NODE_TYPE_ELEMENT;
  step.name = NULL;

  ns = xpath_select_xpath(d.b, &step);
  assert(1 == nodeset_count(ns));
  n = nodeset_get(ns, 0);
  assert(n == d.b);

  nodeset_free(ns);
  destroy_xpath_sibling_test(&d);
}

static void
test_xpath_axis_parent(void)
{
  nodeset_t *ns;
  const node_t *n;
  xpath_step_t step;
  xpath_sibling_test_t d;

  init_xpath_sibling_test(&d);

  step.axis = XPATH_AXIS_PARENT;
  step.type = XPATH_NODE_TYPE_ELEMENT;
  step.name = NULL;

  ns = xpath_select_xpath(d.b, &step);
  assert(1 == nodeset_count(ns));
  n = nodeset_get(ns, 0);
  assert(n == d.one);

  nodeset_free(ns);
  destroy_xpath_sibling_test(&d);
}

static void
test_xpath_axis_following_sibling(void)
{
  nodeset_t *ns;
  const node_t *n;
  xpath_step_t step;
  xpath_sibling_test_t d;

  init_xpath_sibling_test(&d);

  step.axis = XPATH_AXIS_FOLLOWING_SIBLING;
  step.type = XPATH_NODE_TYPE_ELEMENT;
  step.name = NULL;

  ns = xpath_select_xpath(d.b, &step);
  assert(2 == nodeset_count(ns));
  n = nodeset_get(ns, 0);
  assert(n == d.c);
  n = nodeset_get(ns, 1);
  assert(n == d.d);

  nodeset_free(ns);
  destroy_xpath_sibling_test(&d);
}

static void
test_xpath_axis_preceding_sibling(void)
{
  nodeset_t *ns;
  const node_t *n;
  xpath_step_t step;
  xpath_sibling_test_t d;

  init_xpath_sibling_test(&d);

  step.axis = XPATH_AXIS_PRECEDING_SIBLING;
  step.type = XPATH_NODE_TYPE_ELEMENT;
  step.name = NULL;

  ns = xpath_select_xpath(d.d, &step);
  assert(3 == nodeset_count(ns));
  n = nodeset_get(ns, 0);
  assert(n == d.c);
  n = nodeset_get(ns, 1);
  assert(n == d.b);
  n = nodeset_get(ns, 2);
  assert(n == d.a);

  nodeset_free(ns);
  destroy_xpath_sibling_test(&d);
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
  element_t *parent = document_element_new(doc, "parent");
  element_t *children[4];
  nodeset_t *res;

  children[0] = document_element_new(doc, "one");
  children[1] = document_element_new(doc, "two");
  children[2] = document_element_new(doc, "one");
  children[3] = document_element_new(doc, "four");

  node_append_child((node_t *)parent, (node_t *)children[0]);
  node_append_child((node_t *)parent, (node_t *)children[1]);
  node_append_child((node_t *)parent, (node_t *)children[2]);
  node_append_child((node_t *)parent, (node_t *)children[3]);

  res = xpath_apply_xpath((node_t *)parent, name);

  assert(2 == nodeset_count(res));

  assert_nodeset_element_name(res, 0, name);
  assert_nodeset_element_name(res, 1, name);
}

int
main(int argc, char **argv)
{
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
  test_xpath_apply_element();

  return EXIT_SUCCESS;
}
