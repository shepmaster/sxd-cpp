#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "node-test-text-node.h"

TEST_GROUP(NodeTestTextNode)
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

TEST(NodeTestTextNode, does_not_match_an_element)
{
  NodeTestTextNode test;

  CHECK_EQUAL(false, test.include_node(*element));
}

TEST(NodeTestTextNode, matches_a_text_node)
{
  NodeTestTextNode test;

  CHECK_EQUAL(true, test.include_node(*text_node));
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
