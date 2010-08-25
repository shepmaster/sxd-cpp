#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "test-utilities.h"
#include "nodeset.h"

static void
test_new_nodeset(void)
{
  nodeset_t *ns;
  ns = nodeset_new();
  assert(ns != NULL);
  nodeset_free(ns);
}

static void
test_add_node(void)
{
  document_t *doc;
  node_t *n;
  nodeset_t *ns;

  doc = document_new();
  n = element_cast_to_node(document_element_new(doc, "one"));
  ns = nodeset_new();

  nodeset_add(ns, n);
  assert(1 == nodeset_count(ns));

  nodeset_free(ns);
  node_free(n);
  document_free(doc);
}

static void
test_get_node(void)
{
  document_t *doc;
  node_t *n;
  const node_t *n2;
  nodeset_t *ns;

  doc = document_new();
  n = element_cast_to_node(document_element_new(doc, "one"));
  ns = nodeset_new();

  nodeset_add(ns, n);
  n2 = nodeset_get(ns, 0);

  assert(n == n2);

  nodeset_free(ns);  
  node_free(n);
  document_free(doc);
}

static void
test_add_nodeset(void)
{
  document_t *doc;
  node_t *n;
  node_t *n2;
  nodeset_t *ns;
  nodeset_t *ns2;

  doc = document_new();
  n = test_helper_new_node(doc, "one");
  n2 = test_helper_new_node(doc, "two");
  ns = nodeset_new();
  ns2 = nodeset_new();

  nodeset_add(ns, n);
  nodeset_add(ns2, n2);

  nodeset_add_nodeset(ns, ns2);
  assert(2 == nodeset_count(ns));
  assert(n == nodeset_get(ns, 0));
  assert(n2 == nodeset_get(ns, 1));

  nodeset_free(ns);
  nodeset_free(ns2);
  node_free(n);
  node_free(n2);
  document_free(doc);
}

int
main(int argc, char **argv)
{
  test_new_nodeset();
  test_add_node();
  test_get_node();
  test_add_nodeset();

  return EXIT_SUCCESS;
}
