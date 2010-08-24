#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "document.h"
#include "node.h"
#include "test-utilities.h"

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

static void
test_insert_next_sibling(void)
{
  document_t *doc = document_new();
  node_t *a = test_helper_new_node(doc, "a");
  node_t *b = test_helper_new_node(doc, "b");
  node_t *c = test_helper_new_node(doc, "c");

  node_insert_next_sibling(a, c);
  assert(NULL == node_prev_sibling(a));
  assert(c == node_next_sibling(a));
  assert(a == node_prev_sibling(c));
  assert(NULL == node_next_sibling(c));

  node_insert_next_sibling(a, b);
  assert(NULL == node_prev_sibling(a));
  assert(b == node_next_sibling(a));
  assert(a == node_prev_sibling(b));
  assert(c == node_next_sibling(b));
  assert(b == node_prev_sibling(c));
  assert(NULL == node_next_sibling(c));

  node_free(a);
  node_free(b);
  node_free(c);
  document_free(doc);
}

static void
test_append_child_siblings(void)
{
  document_t *doc = document_new();
  node_t *parent = test_helper_new_node(doc, "parent");
  node_t *a = test_helper_new_node(doc, "a");
  node_t *b = test_helper_new_node(doc, "b");
  node_t *c = test_helper_new_node(doc, "c");

  node_append_child(parent, a);
  assert(NULL == node_prev_sibling(a));
  assert(NULL == node_next_sibling(a));

  node_append_child(parent, b);
  assert(NULL == node_prev_sibling(a));
  assert(b == node_next_sibling(a));
  assert(a == node_prev_sibling(b));
  assert(NULL == node_next_sibling(b));

  node_append_child(parent, c);
  assert(NULL == node_prev_sibling(a));
  assert(b == node_next_sibling(a));
  assert(a == node_prev_sibling(b));
  assert(c == node_next_sibling(b));
  assert(b == node_prev_sibling(c));
  assert(NULL == node_next_sibling(c));

  node_free(parent);
  document_free(doc);
}

int
main(int argc, char **argv)
{
  test_append_child();
  test_remove_child();
  test_free_child();
  test_sibling();
  test_insert_next_sibling();
  test_append_child_siblings();

  return EXIT_SUCCESS;
}
