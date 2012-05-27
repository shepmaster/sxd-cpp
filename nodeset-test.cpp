#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "test-utilities.h"
#include "nodeset.h"

TEST_GROUP(nodeset)
{};

TEST(nodeset, new_nodeset)
{
  nodeset_t *ns;
  ns = nodeset_new();
  CHECK(ns != NULL);
  nodeset_free(ns);
}

TEST(nodeset, add_node)
{
  document_t *doc;
  node_t *n;
  nodeset_t *ns;

  doc = document_new();
  n = element_cast_to_node(document_element_new(doc, "one"));
  ns = nodeset_new();

  nodeset_add(ns, n);
  CHECK_EQUAL(1, nodeset_count(ns));

  nodeset_free(ns);
  node_free(n);
  document_free(doc);
}

TEST(nodeset, get_node)
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

  POINTERS_EQUAL(n, n2);

  nodeset_free(ns);  
  node_free(n);
  document_free(doc);
}

TEST(nodeset, add_nodeset)
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
  CHECK_EQUAL(2, nodeset_count(ns));
  POINTERS_EQUAL(n, nodeset_get(ns, 0));
  POINTERS_EQUAL(n2, nodeset_get(ns, 1));

  nodeset_free(ns);
  nodeset_free(ns2);
  node_free(n);
  node_free(n2);
  document_free(doc);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
