#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "document.h"

static void
test_new_document(void)
{
  document_t *doc;
  
  doc = document_new();
  assert(doc != NULL);
  document_free(doc);
}

static void
test_document_managed_count(void)
{
  document_t *doc;
  node_t *n1, *n2, *n3;

  doc = document_new();

  n1 = document_node_new(doc, "alpha");
  assert(document_managed_node_count(doc) == 1);
  n2 = document_node_new(doc, "beta");
  assert(document_managed_node_count(doc) == 2);
  n3 = document_node_new(doc, "omega");
  assert(document_managed_node_count(doc) == 3);

  node_append_child(n1, n2);
  node_append_child(n2, n3);

  node_free(n3);
  assert(document_managed_node_count(doc) == 2);
  node_free(n1);
  assert(document_managed_node_count(doc) == 0);

  document_free(doc);
}

static void
test_move_node_between_documents(void)
{
  document_t *d1, *d2;
  node_t *n;
  const char *orig_name, *orig_attr_value;

  d1 = document_new();
  d2 = document_new();

  n = document_node_new(d1, "hello");
  node_set_attribute(n, "enabled", "false");
  orig_name = node_name(n);
  orig_attr_value = node_get_attribute(n, "enabled");

  assert(document_managed_node_count(d1) == 1);
  assert(document_managed_node_count(d2) == 0);

  document_manage_node(d2, n);
  assert(document_managed_node_count(d1) == 0);
  assert(document_managed_node_count(d2) == 1);
  assert(strcmp(node_name(n), "hello") == 0);
  assert(strcmp(node_get_attribute(n, "enabled"), "false") == 0);
  assert(node_name(n) != orig_name);
  assert(node_get_attribute(n, "enabled") != orig_attr_value);
}

int
main(int argc, char **argv)
{
  test_new_document();
  test_document_managed_count();
  test_move_node_between_documents();

  return EXIT_SUCCESS;
}
