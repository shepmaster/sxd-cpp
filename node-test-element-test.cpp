#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "node-test-element.h"

TEST_GROUP(NodeTestElement)
{
  Document d;
  Node *element;
  Node *text_node;

  void setup() {
    element = d.new_element("element");
    text_node = d.new_text_node("text");
  }

  void teardown() {
    delete element;
    delete text_node;
  }
};

TEST(NodeTestElement, matches_an_element)
{
  NodeTestElement test;

  CHECK_EQUAL(true, test.include_node(*element));
}

TEST(NodeTestElement, does_not_match_a_text_node)
{
  NodeTestElement test;

  CHECK_EQUAL(false, test.include_node(*text_node));
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
