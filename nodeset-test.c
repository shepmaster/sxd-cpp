#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
  node_t *n2;
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

int
main(int argc, char **argv)
{
  test_new_nodeset();
  test_add_node();
  test_get_node();

  return EXIT_SUCCESS;
}
