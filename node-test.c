#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "document.h"
#include "node.h"

static void
test_append_child(void)
{
  document_t *doc;
  node_t *parent;
  node_t *child;

  doc = document_new();
  parent = document_node_new(doc, "parent");
  child = document_node_new(doc, "child");

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
  parent = document_node_new(doc, "parent");
  child = document_node_new(doc, "child");

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
  parent = document_node_new(doc, "parent");
  child = document_node_new(doc, "child");

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
  parent = document_node_new(doc, "parent");
  child1 = document_node_new(doc, "child1");
  child2 = document_node_new(doc, "child2");

  node_append_child(parent, child1);
  node_append_child(parent, child2);

  assert(node_next_sibling(child1) == child2);

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

  return EXIT_SUCCESS;
}
