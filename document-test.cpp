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

TEST_GROUP(document_parse_error)
{
  document_t *doc;
  GError *error;
};

TEST(document_parse_error, basic)
{
  doc = document_parse("<<", &error);
  POINTERS_EQUAL(NULL, doc);
  CHECK(NULL != error);
  CHECK(g_error_matches(error, DOCUMENT_PARSE_ERROR, DOCUMENT_PARSE_ERROR_FAILED));
}

static void
_check_parse_error(GError *error, const char *file, int line)
{
  if (NULL == error) return;
  FAIL_LOCATION(error->message, file, line);
}
#define CHECK_PARSE_ERROR(error)                \
  _check_parse_error(error, __FILE__, __LINE__)

#define CHECK_ELEMENT_NAME(_node, _name)                        \
{                                                               \
  CHECK_EQUAL(NODE_TYPE_ELEMENT, node_type(_node));             \
  STRCMP_EQUAL(_name, element_name((element_t *)_node));        \
}

#define CHECK_TEXT_NODE(_node, _content)                        \
{                                                               \
  CHECK_EQUAL(NODE_TYPE_TEXT_NODE, node_type(_node));           \
  STRCMP_EQUAL(_content, text_node_text((text_node_t *)_node)); \
}

TEST_GROUP(document_parse)
{
  document_t *doc;
  element_t *root;
  GError *error;

  void teardown(void)
  {
    if (doc) document_free(doc);
  }
};

TEST(document_parse, empty)
{
  doc = document_parse("<hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", element_name(root));
}

TEST(document_parse, empty_with_space)
{
  doc = document_parse("<hello />", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", element_name(root));
}

TEST(document_parse, empty_with_end_tag)
{
  doc = document_parse("<hello></hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", element_name(root));
}

TEST(document_parse, element_with_attribute)
{
  doc = document_parse("<hello one='two' />", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", element_name(root));
  STRCMP_EQUAL("two", element_get_attribute(root, "one"));
}

TEST(document_parse, element_with_attributes)
{
  doc = document_parse("<hello one='two' three='four' />", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", element_name(root));
  STRCMP_EQUAL("two", element_get_attribute(root, "one"));
  STRCMP_EQUAL("four", element_get_attribute(root, "three"));
}

TEST(document_parse, element_with_attribute_with_nonalpha)
{
  doc = document_parse("<hello one=\"check one, 2\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", element_name(root));
  STRCMP_EQUAL("check one, 2", element_get_attribute(root, "one"));
}

TEST(document_parse, element_with_attribute_with_entity)
{
  doc = document_parse("<hello one=\"&lt;\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("<", element_get_attribute(root, "one"));
}

TEST(document_parse, element_with_attribute_with_char_ref)
{
  doc = document_parse("<hello one=\"&#114;\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("r", element_get_attribute(root, "one"));
}

TEST(document_parse, element_with_child)
{
  node_t *node;
  doc = document_parse("<hello><world /></hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  node = node_first_child(element_cast_to_node(root));
  STRCMP_EQUAL("hello", element_name(root));
  CHECK_ELEMENT_NAME(node, "world");
}

TEST(document_parse, element_with_two_children)
{
  node_t *node;

  doc = document_parse("<hello><world /><cool /></hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = node_first_child(element_cast_to_node(root));
  CHECK_ELEMENT_NAME(node, "world");

  node = node_next_sibling(node);
  CHECK_ELEMENT_NAME(node, "cool");
}

TEST(document_parse, element_with_grandchild)
{
  node_t *node;

  doc = document_parse("<hello><world><cool /></world></hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = node_first_child((node_t *)root);
  CHECK_ELEMENT_NAME(node, "world");

  node = node_first_child(node);
  CHECK_ELEMENT_NAME(node, "cool");
}

TEST(document_parse, element_with_text)
{
  node_t *node;

  doc = document_parse("<hello>world</hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = node_first_child((node_t *)root);
  CHECK_TEXT_NODE(node, "world");
}

TEST(document_parse, element_with_entities)
{
  node_t *node;

  doc = document_parse("<a>&lt;&gt;&amp;&quot;&apos;</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = node_first_child((node_t *)root);
  CHECK_TEXT_NODE(node, "<");

  node = node_next_sibling(node);
  CHECK_TEXT_NODE(node, ">");

  node = node_next_sibling(node);
  CHECK_TEXT_NODE(node, "&");

  node = node_next_sibling(node);
  CHECK_TEXT_NODE(node, "\"");

  node = node_next_sibling(node);
  CHECK_TEXT_NODE(node, "'");
}

TEST(document_parse, element_with_char_ref)
{
  node_t *node;

  doc = document_parse("<a>&#77;</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = node_first_child((node_t *)root);
  CHECK_TEXT_NODE(node, "M");
}

TEST(document_parse, element_with_char_ref_hex)
{
  node_t *node;

  doc = document_parse("<a>&#x4d;</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = node_first_child((node_t *)root);
  CHECK_TEXT_NODE(node, "M");
}

TEST(document_parse, element_with_nonalpha_text)
{
  node_t *node;

  doc = document_parse("<a>one, 2</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = node_first_child((node_t *)root);
  CHECK_TEXT_NODE(node, "one, 2");
}

TEST(document_parse, element_with_mixed_content)
{
  node_t *node;

  doc = document_parse("<hello><one />b</hello>", &error);
  CHECK_PARSE_ERROR(error);

  root = document_root(doc);

  node = node_first_child((node_t *)root);
  CHECK_ELEMENT_NAME(node, "one");

  node = node_next_sibling(node);
  CHECK_TEXT_NODE(node, "b");
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
