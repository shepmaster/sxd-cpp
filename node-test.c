#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "document.h"
#include "node.h"

static node_t *
test_helper_new_node(document_t *doc, const char * const name)
{
  element_t *e;
  e = document_element_new(doc, name);
  return element_cast_to_node(e);
}

static node_t *
test_helper_new_text_node(document_t *doc, const char * const text)
{
  text_node_t *tn;
  tn = document_text_node_new(doc, text);
  return text_node_cast_to_node(tn);
}

static void
test_append_child(void)
{
  document_t *doc;
  node_t *parent;
  node_t *child;

  doc = document_new();
  parent = test_helper_new_node(doc, "parent");
  child = test_helper_new_node(doc, "child");

  node_append_child(parent, child);
  assert(node_first_child(parent) == child);
  assert(node_parent(child) == parent);

  node_free(parent);
  document_free(doc);
}

static void
test_remove_child(void)
{
  document_t *doc;
  node_t *parent;
  node_t *child;

  doc = document_new();
  parent = test_helper_new_node(doc, "parent");
  child = test_helper_new_node(doc, "child");

  node_append_child(parent, child);
  node_remove_child(parent, child);
  assert(node_first_child(parent) == NULL);

  node_free(parent);
  node_free(child);
  document_free(doc);
}

static void
test_free_child(void)
{
  document_t *doc;
  node_t *parent;
  node_t *child;

  doc = document_new();
  parent = test_helper_new_node(doc, "parent");
  child = test_helper_new_node(doc, "child");

  node_append_child(parent, child);
  node_free(child);
  assert(node_first_child(parent) == NULL);

  node_free(parent);
  document_free(doc);
}

static void
test_sibling(void)
{
  document_t *doc;
  node_t *parent;
  node_t *child1;
  node_t *child2;

  doc = document_new();
  parent = test_helper_new_node(doc, "parent");
  child1 = test_helper_new_node(doc, "child1");
  child2 = test_helper_new_node(doc, "child2");

  node_append_child(parent, child1);
  node_append_child(parent, child2);

  assert(node_next_sibling(child1) == child2);

  node_free(parent);
  document_free(doc);
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

  init_xpath_test(&d);

  ns = node_select_xpath(d.parent, XPATH_PREDICATE_ELEMENT, NULL);
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

  init_xpath_test(&d);

  ns = node_select_xpath(d.parent, XPATH_PREDICATE_TEXT_NODE, NULL);
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

  init_xpath_test(&d);

  ns = node_select_xpath(d.parent, XPATH_PREDICATE_ELEMENT | XPATH_PREDICATE_TEXT_NODE, NULL);
  assert(2 == nodeset_count(ns));
  n = nodeset_get(ns, 0);
  assert(n == d.e);
  n = nodeset_get(ns, 1);
  assert(n == d.tn);

  nodeset_free(ns);
  destroy_xpath_test(&d);
}

int
main(int argc, char **argv)
{
  test_append_child();
  test_remove_child();
  test_free_child();
  test_sibling();
  test_xpath_element();
  test_xpath_text_node();
  test_xpath_element_and_text_node();

  return EXIT_SUCCESS;
}
