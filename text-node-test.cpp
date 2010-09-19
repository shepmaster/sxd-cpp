#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

extern "C" {
#include "document.h"
#include "text-node.h"

#include "test-utilities.h"
}

TEST_GROUP(text_node)
{};

TEST(text_node, new_text_node)
{
  document_t *doc;
  text_node_t *tn;

  doc = document_new();
  tn = document_text_node_new(doc, "I am text");
  STRCMP_EQUAL("I am text", text_node_text(tn));
  text_node_free(tn);
  document_free(doc);
}

TEST(text_node, output)
{
  document_t *doc;
  text_node_t *tn;
  test_output_t to;

  test_output_init(&to);

  doc = document_new();
  tn = document_text_node_new(doc, "I am text");

  text_node_output(tn, &to.out);
  STRCMP_EQUAL("I am text", to.string->str);

  text_node_free(tn);
  document_free(doc);
  test_output_destroy(&to);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
