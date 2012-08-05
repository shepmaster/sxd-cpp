#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "test-utilities.h"
#include "document.h"
#include "nodeset.h"

TEST_GROUP(nodeset)
{
  Document doc;
  Node *n;
  Nodeset ns;

  void setup() {
    n = doc.new_element("one");
  }

  void teardown() {
    delete n;
  }
};

TEST(nodeset, add_node)
{
  ns.add(n);
  CHECK_EQUAL(1, ns.count());
}

TEST(nodeset, get_node)
{
  const Node *n2;

  ns.add(n);
  n2 = ns[0];

  POINTERS_EQUAL(n, n2);
}

TEST(nodeset, add_nodeset)
{
  Node *n2;
  Nodeset ns2;

  n2 = test_helper_new_node(doc, "two");

  ns.add(n);
  ns2.add(n2);

  ns.add_nodeset(ns2);
  CHECK_EQUAL(2, ns.count());
  POINTERS_EQUAL(n, ns[0]);
  POINTERS_EQUAL(n2, ns[1]);

  delete n2;
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
