#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "node-test-named-element.h"

TEST_GROUP(NodeTestNamedElement)
{
  Document d;
  Node *element;

  void setup() {
    element = d.new_element("element");
  }

  void teardown() {
    delete element;
  }
};

TEST(NodeTestNamedElement, matches_element_of_same_name)
{
  NodeTestNamedElement test("element");

  CHECK_EQUAL(true, test.include_node(*element));
}

TEST(NodeTestNamedElement, does_not_match_element_of_different_name)
{
  NodeTestNamedElement test("wrong");

  CHECK_EQUAL(false, test.include_node(*element));
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
