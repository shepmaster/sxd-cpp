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
  document_t *doc;
  TextNode *tn;

  doc = document_new();
  tn = document_text_node_new(doc, "I am text");
  STRCMP_EQUAL("I am text", tn->text());
  delete tn;
  document_free(doc);
}

TEST(text_node, output)
{
  document_t *doc;
  TextNode *tn;
  StringOutput out;

  doc = document_new();
  tn = document_text_node_new(doc, "I am text");

  tn->output(out);
  STRCMP_EQUAL("I am text", out.string());

  delete tn;
  document_free(doc);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
