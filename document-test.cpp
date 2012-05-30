#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness_c.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "test-utilities.h"
#include "document.h"

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
  Element *n1, *n2, *n3;

  doc = document_new();

  n1 = document_element_new(doc, "alpha");
  CHECK_EQUAL(1, document_managed_node_count(doc));
  n2 = document_element_new(doc, "beta");
  CHECK_EQUAL(2, document_managed_node_count(doc));
  n3 = document_element_new(doc, "omega");
  CHECK_EQUAL(3, document_managed_node_count(doc));

  n1->append_child(n2);
  n2->append_child(n3);

  delete n3;
  CHECK_EQUAL(2, document_managed_node_count(doc));
  delete n1;
  CHECK_EQUAL(0, document_managed_node_count(doc));

  document_free(doc);
}

TEST(document, move_node_between_documents)
{
  document_t *d1, *d2;
  Element *n;
  const char *orig_name, *orig_attr_value;

  d1 = document_new();
  d2 = document_new();

  n = document_element_new(d1, "hello");
  n->set_attribute("enabled", "false");
  orig_name = n->name();
  orig_attr_value = n->get_attribute("enabled");

  CHECK_EQUAL(1, document_managed_node_count(d1));
  CHECK_EQUAL(0, document_managed_node_count(d2));

  document_manage_node(d2, n);
  CHECK_EQUAL(0, document_managed_node_count(d1));
  CHECK_EQUAL(1, document_managed_node_count(d2));
  STRCMP_EQUAL(n->name(), "hello");
  STRCMP_EQUAL(n->get_attribute("enabled"), "false");
  CHECK(n->name() != orig_name);
  CHECK(n->get_attribute("enabled") != orig_attr_value);

  delete n;
  document_free(d1);
  document_free(d2);
}

TEST_GROUP(document_parse_error)
{
  document_t *doc;
  GError *error;

  void teardown(void)
  {
    if (doc) {
      delete document_root(doc);
      document_free(doc);
    }
    if (error) g_error_free(error);
  }
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
  CHECK_EQUAL(NODE_TYPE_ELEMENT, _node->type());             \
  STRCMP_EQUAL(_name, ((Element *)_node)->name());         \
}

#define CHECK_TEXT_NODE(_node, _content)                        \
{                                                               \
  CHECK_EQUAL(NODE_TYPE_TEXT_NODE, _node->type());           \
  STRCMP_EQUAL(_content, ((TextNode *)_node)->text());    \
}

TEST_GROUP(document_parse)
{
  document_t *doc;
  Element *root;
  GError *error;

  void teardown(void)
  {
    if (doc) {
      delete document_root(doc);
      document_free(doc);
    }
    if (error) g_error_free(error);
  }
};

TEST(document_parse, empty)
{
  doc = document_parse("<hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", root->name());
}

TEST(document_parse, empty_with_space)
{
  doc = document_parse("<hello />", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", root->name());
}

TEST(document_parse, empty_with_end_tag)
{
  doc = document_parse("<hello></hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", root->name());
}

TEST(document_parse, preamble)
{
  doc = document_parse("<?xml?><hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", root->name());
}

TEST(document_parse, preamble_with_space)
{
  doc = document_parse("<?xml?>\n<hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", root->name());
}

TEST(document_parse, preamble_with_version)
{
  doc = document_parse("<?xml version='1.0' ?><hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", root->name());
}

TEST(document_parse, empty_with_leading_whitespace)
{
  doc = document_parse("\n\r \t<hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", root->name());
}

TEST(document_parse, element_with_attribute)
{
  doc = document_parse("<hello one='two' />", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", root->name());
  STRCMP_EQUAL("two", root->get_attribute("one"));
}

TEST(document_parse, element_with_attributes)
{
  doc = document_parse("<hello one='two' three='four' />", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", root->name());
  STRCMP_EQUAL("two", root->get_attribute("one"));
  STRCMP_EQUAL("four", root->get_attribute("three"));
}

TEST(document_parse, element_with_attribute_with_nonalpha)
{
  doc = document_parse("<hello one=\"check one, 2\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("hello", root->name());
  STRCMP_EQUAL("check one, 2", root->get_attribute("one"));
}

TEST(document_parse, element_with_attribute_with_entity)
{
  doc = document_parse("<hello one=\"&lt;\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("<", root->get_attribute("one"));
}

TEST(document_parse, element_with_attribute_with_char_ref)
{
  doc = document_parse("<hello one=\"&#114;\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("r", root->get_attribute("one"));
}

TEST(document_parse, element_with_attribute_with_char_ref_hex)
{
  doc = document_parse("<hello one=\"&#x63;\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  STRCMP_EQUAL("c", root->get_attribute("one"));
}

TEST(document_parse, element_with_child)
{
  node_t *node;
  doc = document_parse("<hello><world /></hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);
  node = root->first_child();
  STRCMP_EQUAL("hello", root->name());
  CHECK_ELEMENT_NAME(node, "world");
}

TEST(document_parse, element_with_two_children)
{
  node_t *node;

  doc = document_parse("<hello><world /><cool /></hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = root->first_child();
  CHECK_ELEMENT_NAME(node, "world");

  node = node->next_sibling();
  CHECK_ELEMENT_NAME(node, "cool");
}

TEST(document_parse, element_with_two_children_first_empty)
{
  node_t *node;

  doc = document_parse("<a><b></b><c/></a>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = root->first_child();
  CHECK_ELEMENT_NAME(node, "b");

  node = node->next_sibling();
  CHECK_ELEMENT_NAME(node, "c");
}

TEST(document_parse, element_with_two_children_whitespace)
{
  node_t *node;

  doc = document_parse("<hello>\r\n\t<world />\n  <cool />\n</hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = root->first_child();
  CHECK_ELEMENT_NAME(node, "world");

  node = node->next_sibling();
  CHECK_ELEMENT_NAME(node, "cool");
}

TEST(document_parse, element_with_grandchild)
{
  node_t *node;

  doc = document_parse("<hello><world><cool /></world></hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = root->first_child();
  CHECK_ELEMENT_NAME(node, "world");

  node = node->first_child();
  CHECK_ELEMENT_NAME(node, "cool");
}

TEST(document_parse, element_with_text)
{
  node_t *node;

  doc = document_parse("<hello>world</hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = root->first_child();
  CHECK_TEXT_NODE(node, "world");
}

TEST(document_parse, element_with_entities)
{
  node_t *node;

  doc = document_parse("<a>&lt;&gt;&amp;&quot;&apos;</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = root->first_child();
  CHECK_TEXT_NODE(node, "<");

  node = node->next_sibling();
  CHECK_TEXT_NODE(node, ">");

  node = node->next_sibling();
  CHECK_TEXT_NODE(node, "&");

  node = node->next_sibling();
  CHECK_TEXT_NODE(node, "\"");

  node = node->next_sibling();
  CHECK_TEXT_NODE(node, "'");
}

TEST(document_parse, element_with_char_ref)
{
  node_t *node;

  doc = document_parse("<a>&#77;</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = root->first_child();
  CHECK_TEXT_NODE(node, "M");
}

TEST(document_parse, element_with_char_ref_hex)
{
  node_t *node;

  doc = document_parse("<a>&#x4d;</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = root->first_child();
  CHECK_TEXT_NODE(node, "M");
}

TEST(document_parse, element_with_nonalpha_text)
{
  node_t *node;

  doc = document_parse("<a>one, 2</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = document_root(doc);

  node = root->first_child();
  CHECK_TEXT_NODE(node, "one, 2");
}

TEST(document_parse, element_with_mixed_content)
{
  node_t *node;

  doc = document_parse("<hello><one />b</hello>", &error);
  CHECK_PARSE_ERROR(error);

  root = document_root(doc);

  node = root->first_child();
  CHECK_ELEMENT_NAME(node, "one");

  node = node->next_sibling();
  CHECK_TEXT_NODE(node, "b");
}

TEST(document_parse, comment)
{
  node_t *node;

  doc = document_parse("<a><!--alert--></a>", &error);
  CHECK_PARSE_ERROR(error);

  root = document_root(doc);

  node = root->first_child();
  CHECK_EQUAL(NODE_TYPE_COMMENT, node->type());
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
