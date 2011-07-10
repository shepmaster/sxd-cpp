#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness_c.h>
#include <CppUTest/CommandLineTestRunner.h>

extern "C" {
#include "document.h"
}

TEST_GROUP(document)
{};

TEST(document, new_document)
{
  document_t *doc;
  
  doc = document_new();
  CHECK(doc != NULL);
  document_free(doc);
}

TEST(document, managed_count)
{
  document_t *doc;
  element_t *n1, *n2, *n3;

  doc = document_new();

  n1 = document_element_new(doc, "alpha");
  CHECK_EQUAL(1, document_managed_node_count(doc));
  n2 = document_element_new(doc, "beta");
  CHECK_EQUAL(2, document_managed_node_count(doc));
  n3 = document_element_new(doc, "omega");
  CHECK_EQUAL(3, document_managed_node_count(doc));

  node_append_child(element_cast_to_node(n1), element_cast_to_node(n2));
  node_append_child(element_cast_to_node(n2), element_cast_to_node(n3));

  element_free(n3);
  CHECK_EQUAL(2, document_managed_node_count(doc));
  element_free(n1);
  CHECK_EQUAL(0, document_managed_node_count(doc));

  document_free(doc);
}

TEST(document, move_node_between_documents)
{
  document_t *d1, *d2;
  element_t *n;
  const char *orig_name, *orig_attr_value;

  d1 = document_new();
  d2 = document_new();

  n = document_element_new(d1, "hello");
  element_set_attribute(n, "enabled", "false");
  orig_name = element_name(n);
  orig_attr_value = element_get_attribute(n, "enabled");

  CHECK_EQUAL(1, document_managed_node_count(d1));
  CHECK_EQUAL(0, document_managed_node_count(d2));

  document_manage_node(d2, element_cast_to_node(n));
  CHECK_EQUAL(0, document_managed_node_count(d1));
  CHECK_EQUAL(1, document_managed_node_count(d2));
  STRCMP_EQUAL(element_name(n), "hello");
  STRCMP_EQUAL(element_get_attribute(n, "enabled"), "false");
  CHECK(element_name(n) != orig_name);
  CHECK(element_get_attribute(n, "enabled") != orig_attr_value);

  element_free(n);
  document_free(d1);
  document_free(d2);
}

TEST(document, parse_simple)
{
  document_t *doc;
  element_t *root;

  doc = document_parse("<hello/>");
  root = document_root(doc);
  STRCMP_EQUAL("hello", element_name(root));

  document_free(doc);
}

TEST(document, parse_simple_with_space)
{
  document_t *doc;
  element_t *root;

  doc = document_parse("<hello />");
  root = document_root(doc);
  STRCMP_EQUAL("hello", element_name(root));

  document_free(doc);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
