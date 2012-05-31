#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "test-utilities.h"
#include "document.h"
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
  Document doc;
  Node *n;
  nodeset_t *ns;

  n = doc.new_element("one");
  ns = nodeset_new();

  nodeset_add(ns, n);
  CHECK_EQUAL(1, nodeset_count(ns));

  nodeset_free(ns);
  delete n;
}

TEST(nodeset, get_node)
{
  Document doc;
  Node *n;
  const Node *n2;
  nodeset_t *ns;

  n = doc.new_element("one");
  ns = nodeset_new();

  nodeset_add(ns, n);
  n2 = nodeset_get(ns, 0);

  POINTERS_EQUAL(n, n2);

  nodeset_free(ns);  
  delete n;
}

TEST(nodeset, add_nodeset)
{
  Document doc;
  Node *n;
  Node *n2;
  nodeset_t *ns;
  nodeset_t *ns2;

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
  delete n;
  delete n2;
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
