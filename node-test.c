#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "document.h"
#include "node.h"

static void
test_new_node(void)
{
  document_t *doc;
  node_t *node;
  const char * const name = "hello";

  doc = document_new();
  node = document_node_new(doc, name);
  assert(node != NULL);
  assert(strcmp(node_name(node), name) == 0);
  assert(node_document(node) == doc);
  node_free(node);
}

static void
test_mutated_name(void)
{
  document_t *doc;
  node_t *node;
  char * name;
 
  name = strdup("hello");
  doc = document_new();
  node = document_node_new(doc, name);

  name[0] = 'y';
  assert(strcmp(node_name(node), "hello") == 0);

  node_free(node);
  document_free(doc);
  free(name);
}

static void
test_set_attribute(void)
{
  document_t *doc;
  node_t *node;
  const char * const node_name = "hello";
  const char * const attr_name = "type";
  const char * const attr_value = "world";

  doc = document_new();
  node = document_node_new(doc, node_name);

  node_set_attribute(node, attr_name, attr_value);
  assert(strcmp(node_get_attribute(node, attr_name), attr_value) == 0);

  node_free(node);
  document_free(doc);
}

static void
test_mutated_attribute(void)
{
  document_t *doc;
  node_t *node;
  const char * const node_name = "hello";
  char *attr_name;
  char *attr_value;

  attr_name = strdup("type");
  attr_value = strdup("world");
  doc = document_new();
  node = document_node_new(doc, node_name);

  node_set_attribute(node, attr_name, attr_value);
  
  attr_name[0] = 'h';
  attr_value[0] = 'y';
  assert(strcmp(node_get_attribute(node, "type"), "world") == 0);

  node_free(node);
  document_free(doc);
  free(attr_name);
  free(attr_value);
}

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
  test_new_node();
  test_mutated_name();
  test_set_attribute();
  test_mutated_attribute();
  test_append_child();
  test_remove_child();
  test_free_child();
  test_sibling();

  return EXIT_SUCCESS;
}
