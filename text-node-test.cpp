#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "text-node.h"

#include "test-utilities.h"

TEST_GROUP(text_node)
{};

TEST(text_node, new_text_node)
{
  Document doc;
  TextNode *tn;

  tn = doc.new_text_node("I am text");
  STRCMP_EQUAL("I am text", tn->text());
  delete tn;
}

TEST(text_node, output)
{
  Document doc;
  TextNode *tn;
  StringOutput out;

  tn = doc.new_text_node("I am text");

  tn->output(out);
  STRCMP_EQUAL("I am text", out.string());

  delete tn;
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
