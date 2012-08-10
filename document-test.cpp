#include <iostream>
#include <string.h>

#include <CppUTest/TestHarness_c.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "test-utilities.h"
#include "document.h"

TEST_GROUP(document)
{};

TEST(document, managed_count)
{
  Document doc;
  Element *n1, *n2, *n3;

  n1 = doc.new_element("alpha");
  CHECK_EQUAL(1, doc.managed_node_count());
  n2 = doc.new_element("beta");
  CHECK_EQUAL(2, doc.managed_node_count());
  n3 = doc.new_element("omega");
  CHECK_EQUAL(3, doc.managed_node_count());

  n1->append_child(n2);
  n2->append_child(n3);

  delete n3;
  CHECK_EQUAL(2, doc.managed_node_count());
  delete n1;
  CHECK_EQUAL(0, doc.managed_node_count());
}

TEST(document, move_node_between_documents)
{
  Document d1, d2;
  Element *n;
  const char *orig_name, *orig_attr_value;

  n = d1.new_element("hello");
  n->set_attribute("enabled", "false");
  orig_name = n->name();
  orig_attr_value = n->get_attribute("enabled");

  CHECK_EQUAL(1, d1.managed_node_count());
  CHECK_EQUAL(0, d2.managed_node_count());

  d2.manage_node(n);
  CHECK_EQUAL(0, d1.managed_node_count());
  CHECK_EQUAL(1, d2.managed_node_count());
  STRCMP_EQUAL(n->name(), "hello");
  STRCMP_EQUAL(n->get_attribute("enabled"), "false");
  CHECK(n->name() != orig_name);
  CHECK(n->get_attribute("enabled") != orig_attr_value);

  delete n;
}

TEST_GROUP(document_parse_error)
{
  Document *doc;
  GError *error;

  void teardown(void)
  {
    if (doc) {
      delete doc->root();
      delete doc;
    }
    if (error) g_error_free(error);
  }
};

TEST(document_parse_error, basic)
{
  doc = Document::parse("<<", &error);
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
  Document *doc;
  Element *root;
  GError *error;

  void teardown(void)
  {
    if (doc) {
      delete doc->root();
      delete doc;
    }
    if (error) g_error_free(error);
  }
};

TEST(document_parse, empty)
{
  doc = Document::parse("<hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  STRCMP_EQUAL("hello", root->name());
}

TEST(document_parse, empty_with_space)
{
  doc = Document::parse("<hello />", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  STRCMP_EQUAL("hello", root->name());
}

TEST(document_parse, empty_with_end_tag)
{
  doc = Document::parse("<hello></hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  STRCMP_EQUAL("hello", root->name());
}

TEST(document_parse, preamble)
{
  doc = Document::parse("<?xml?><hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  STRCMP_EQUAL("hello", root->name());
}

TEST(document_parse, preamble_with_space)
{
  doc = Document::parse("<?xml?>\n<hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  STRCMP_EQUAL("hello", root->name());
}

TEST(document_parse, preamble_with_version)
{
  doc = Document::parse("<?xml version='1.0' ?><hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  STRCMP_EQUAL("hello", root->name());
}

TEST(document_parse, empty_with_leading_whitespace)
{
  doc = Document::parse("\n\r \t<hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  STRCMP_EQUAL("hello", root->name());
}

TEST(document_parse, element_with_attribute)
{
  doc = Document::parse("<hello one='two' />", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  STRCMP_EQUAL("hello", root->name());
  STRCMP_EQUAL("two", root->get_attribute("one"));
}

TEST(document_parse, element_with_attributes)
{
  doc = Document::parse("<hello one='two' three='four' />", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  STRCMP_EQUAL("hello", root->name());
  STRCMP_EQUAL("two", root->get_attribute("one"));
  STRCMP_EQUAL("four", root->get_attribute("three"));
}

TEST(document_parse, element_with_attribute_with_nonalpha)
{
  doc = Document::parse("<hello one=\"check one, 2\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  STRCMP_EQUAL("hello", root->name());
  STRCMP_EQUAL("check one, 2", root->get_attribute("one"));
}

TEST(document_parse, element_with_attribute_with_entity)
{
  doc = Document::parse("<hello one=\"&lt;\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  STRCMP_EQUAL("<", root->get_attribute("one"));
}

TEST(document_parse, element_with_attribute_with_char_ref)
{
  doc = Document::parse("<hello one=\"&#114;\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  STRCMP_EQUAL("r", root->get_attribute("one"));
}

TEST(document_parse, element_with_attribute_with_char_ref_hex)
{
  doc = Document::parse("<hello one=\"&#x63;\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  STRCMP_EQUAL("c", root->get_attribute("one"));
}

TEST(document_parse, element_with_child)
{
  Node *node;
  doc = Document::parse("<hello><world /></hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  node = root->first_child();
  STRCMP_EQUAL("hello", root->name());
  CHECK_ELEMENT_NAME(node, "world");
}

TEST(document_parse, element_with_two_children)
{
  Node *node;

  doc = Document::parse("<hello><world /><cool /></hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();

  node = root->first_child();
  CHECK_ELEMENT_NAME(node, "world");

  node = node->next_sibling();
  CHECK_ELEMENT_NAME(node, "cool");
}

TEST(document_parse, element_with_two_children_first_empty)
{
  Node *node;

  doc = Document::parse("<a><b></b><c/></a>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();

  node = root->first_child();
  CHECK_ELEMENT_NAME(node, "b");

  node = node->next_sibling();
  CHECK_ELEMENT_NAME(node, "c");
}

TEST(document_parse, element_with_two_children_whitespace)
{
  Node *node;

  doc = Document::parse("<hello>\r\n\t<world />\n  <cool />\n</hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();

  node = root->first_child();
  CHECK_ELEMENT_NAME(node, "world");

  node = node->next_sibling();
  CHECK_ELEMENT_NAME(node, "cool");
}

TEST(document_parse, element_with_grandchild)
{
  Node *node;

  doc = Document::parse("<hello><world><cool /></world></hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();

  node = root->first_child();
  CHECK_ELEMENT_NAME(node, "world");

  node = node->first_child();
  CHECK_ELEMENT_NAME(node, "cool");
}

TEST(document_parse, element_with_text)
{
  Node *node;

  doc = Document::parse("<hello>world</hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();

  node = root->first_child();
  CHECK_TEXT_NODE(node, "world");
}

TEST(document_parse, element_with_entities)
{
  Node *node;

  doc = Document::parse("<a>&lt;&gt;&amp;&quot;&apos;</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();

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
  Node *node;

  doc = Document::parse("<a>&#77;</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();

  node = root->first_child();
  CHECK_TEXT_NODE(node, "M");
}

TEST(document_parse, element_with_char_ref_hex)
{
  Node *node;

  doc = Document::parse("<a>&#x4d;</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();

  node = root->first_child();
  CHECK_TEXT_NODE(node, "M");
}

TEST(document_parse, element_with_nonalpha_text)
{
  Node *node;

  doc = Document::parse("<a>one, 2</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();

  node = root->first_child();
  CHECK_TEXT_NODE(node, "one, 2");
}

TEST(document_parse, element_with_mixed_content)
{
  Node *node;

  doc = Document::parse("<hello><one />b</hello>", &error);
  CHECK_PARSE_ERROR(error);

  root = doc->root();

  node = root->first_child();
  CHECK_ELEMENT_NAME(node, "one");

  node = node->next_sibling();
  CHECK_TEXT_NODE(node, "b");
}

TEST(document_parse, comment)
{
  Node *node;

  doc = Document::parse("<a><!--alert--></a>", &error);
  CHECK_PARSE_ERROR(error);

  root = doc->root();

  node = root->first_child();
  CHECK_EQUAL(NODE_TYPE_COMMENT, node->type());
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
