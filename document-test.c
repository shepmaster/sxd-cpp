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
  element_t *n1, *n2, *n3;

  doc = document_new();

  n1 = document_element_new(doc, "alpha");
  assert(document_managed_node_count(doc) == 1);
  n2 = document_element_new(doc, "beta");
  assert(document_managed_node_count(doc) == 2);
  n3 = document_element_new(doc, "omega");
  assert(document_managed_node_count(doc) == 3);

  node_append_child(element_cast_to_node(n1), element_cast_to_node(n2));
  node_append_child(element_cast_to_node(n2), element_cast_to_node(n3));

  element_free(n3);
  assert(document_managed_node_count(doc) == 2);
  element_free(n1);
  assert(document_managed_node_count(doc) == 0);

  document_free(doc);
}

static void
test_move_node_between_documents(void)
{
  document_t *d1, *d2;
  element_t *n;
  const char *orig_name, *orig_attr_value;

  d1 = document_new();
  d2 = document_new();

  n = document_element_new(d1, "hello");
  element_set_attribute(n, "enabled", "false");
  orig_name = element_name(n);
  orig_attr_value = element_get_attribute(n, "enabled");

  assert(document_managed_node_count(d1) == 1);
  assert(document_managed_node_count(d2) == 0);

  document_manage_node(d2, element_cast_to_node(n));
  assert(document_managed_node_count(d1) == 0);
  assert(document_managed_node_count(d2) == 1);
  assert(strcmp(element_name(n), "hello") == 0);
  assert(strcmp(element_get_attribute(n, "enabled"), "false") == 0);
  assert(element_name(n) != orig_name);
  assert(element_get_attribute(n, "enabled") != orig_attr_value);
}

int
main(int argc, char **argv)
{
  test_new_document();
  test_document_managed_count();
  test_move_node_between_documents();

  return EXIT_SUCCESS;
}
