#include "document.h"

//#include "test-utilities.h"

#include "gmock/gmock.h"
#include <iostream>

class DocumentTest : public ::testing::Test
{};

TEST_F(DocumentTest, managed_count)
{
  Document doc;
  Element *n1, *n2, *n3;

  n1 = doc.new_element("alpha");
  ASSERT_EQ(1, doc.managed_node_count());
  n2 = doc.new_element("beta");
  ASSERT_EQ(2, doc.managed_node_count());
  n3 = doc.new_element("omega");
  ASSERT_EQ(3, doc.managed_node_count());

  n1->append_child(n2);
  n2->append_child(n3);

  delete n3;
  ASSERT_EQ(2, doc.managed_node_count());
  delete n1;
  ASSERT_EQ(0, doc.managed_node_count());
}

TEST_F(DocumentTest, move_node_between_documents)
{
  Document d1, d2;
  Element *n;

  n = d1.new_element("hello");
  n->set_attribute("enabled", "false");

  ASSERT_EQ(1, d1.managed_node_count());
  ASSERT_EQ(0, d2.managed_node_count());

  d2.manage_node(n);
  ASSERT_EQ(0, d1.managed_node_count());
  ASSERT_EQ(1, d2.managed_node_count());
  ASSERT_STREQ(n->name(), "hello");
  ASSERT_STREQ(n->get_attribute("enabled"), "false");

  delete n;
}

class DocumentParseErrorTest : public ::testing::Test {
protected:
  Document *doc;
  GError *error = NULL;

  void TearDown(void)
  {
    if (doc) {
      delete doc->root();
      delete doc;
    }
    if (error) g_error_free(error);
  }
};

TEST_F(DocumentParseErrorTest, basic)
{
  doc = Document::parse("<<", &error);
  ASSERT_EQ(NULL, doc);
  ASSERT_NE(nullptr, error);
  ASSERT_TRUE(g_error_matches(error, DOCUMENT_PARSE_ERROR, DOCUMENT_PARSE_ERROR_FAILED));
}

static void
_check_parse_error(GError *error, const char *file, int line)
{
  if (NULL == error) return;
  ADD_FAILURE_AT(file, line) << error->message;
}
#define CHECK_PARSE_ERROR(error)                \
  _check_parse_error(error, __FILE__, __LINE__)

#define CHECK_ELEMENT_NAME(_node, _name)                        \
{                                                               \
  ASSERT_EQ(NODE_TYPE_ELEMENT, _node->type());             \
  ASSERT_STREQ(_name, ((Element *)_node)->name());         \
}

#define CHECK_TEXT_NODE(_node, _content)                        \
{                                                               \
  ASSERT_EQ(NODE_TYPE_TEXT_NODE, _node->type());           \
  ASSERT_STREQ(_content, ((TextNode *)_node)->text());    \
}

class DocumentParseTest : public ::testing::Test {
protected:
  Document *doc;
  Element *root;
  GError *error = NULL;

  void TearDown(void)
  {
    if (doc) {
      delete doc->root();
      delete doc;
    }
    if (error) g_error_free(error);
  }
};

TEST_F(DocumentParseTest, empty)
{
  doc = Document::parse("<hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  ASSERT_STREQ("hello", root->name());
}

TEST_F(DocumentParseTest, empty_with_space)
{
  doc = Document::parse("<hello />", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  ASSERT_STREQ("hello", root->name());
}

TEST_F(DocumentParseTest, empty_with_end_tag)
{
  doc = Document::parse("<hello></hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  ASSERT_STREQ("hello", root->name());
}

TEST_F(DocumentParseTest, preamble)
{
  doc = Document::parse("<?xml?><hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  ASSERT_STREQ("hello", root->name());
}

TEST_F(DocumentParseTest, preamble_with_space)
{
  doc = Document::parse("<?xml?>\n<hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  ASSERT_STREQ("hello", root->name());
}

TEST_F(DocumentParseTest, preamble_with_version)
{
  doc = Document::parse("<?xml version='1.0' ?><hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  ASSERT_STREQ("hello", root->name());
}

TEST_F(DocumentParseTest, empty_with_leading_whitespace)
{
  doc = Document::parse("\n\r \t<hello/>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  ASSERT_STREQ("hello", root->name());
}

TEST_F(DocumentParseTest, element_with_attribute)
{
  doc = Document::parse("<hello one='two' />", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  ASSERT_STREQ("hello", root->name());
  ASSERT_STREQ("two", root->get_attribute("one"));
}

TEST_F(DocumentParseTest, element_with_attributes)
{
  doc = Document::parse("<hello one='two' three='four' />", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  ASSERT_STREQ("hello", root->name());
  ASSERT_STREQ("two", root->get_attribute("one"));
  ASSERT_STREQ("four", root->get_attribute("three"));
}

TEST_F(DocumentParseTest, element_with_attribute_with_nonalpha)
{
  doc = Document::parse("<hello one=\"check one, 2\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  ASSERT_STREQ("hello", root->name());
  ASSERT_STREQ("check one, 2", root->get_attribute("one"));
}

TEST_F(DocumentParseTest, element_with_attribute_with_entity)
{
  doc = Document::parse("<hello one=\"&lt;\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  ASSERT_STREQ("<", root->get_attribute("one"));
}

TEST_F(DocumentParseTest, element_with_attribute_with_char_ref)
{
  doc = Document::parse("<hello one=\"&#114;\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  ASSERT_STREQ("r", root->get_attribute("one"));
}

TEST_F(DocumentParseTest, element_with_attribute_with_char_ref_hex)
{
  doc = Document::parse("<hello one=\"&#x63;\" />", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  ASSERT_STREQ("c", root->get_attribute("one"));
}

TEST_F(DocumentParseTest, element_with_child)
{
  Node *node;
  doc = Document::parse("<hello><world /></hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();
  node = root->first_child();
  ASSERT_STREQ("hello", root->name());
  CHECK_ELEMENT_NAME(node, "world");
}

TEST_F(DocumentParseTest, element_with_two_children)
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

TEST_F(DocumentParseTest, element_with_two_children_first_empty)
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

TEST_F(DocumentParseTest, element_with_two_children_whitespace)
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

TEST_F(DocumentParseTest, element_with_grandchild)
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

TEST_F(DocumentParseTest, element_with_text)
{
  Node *node;

  doc = Document::parse("<hello>world</hello>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();

  node = root->first_child();
  CHECK_TEXT_NODE(node, "world");
}

TEST_F(DocumentParseTest, element_with_entities)
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

TEST_F(DocumentParseTest, element_with_char_ref)
{
  Node *node;

  doc = Document::parse("<a>&#77;</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();

  node = root->first_child();
  CHECK_TEXT_NODE(node, "M");
}

TEST_F(DocumentParseTest, element_with_char_ref_hex)
{
  Node *node;

  doc = Document::parse("<a>&#x4d;</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();

  node = root->first_child();
  CHECK_TEXT_NODE(node, "M");
}

TEST_F(DocumentParseTest, element_with_nonalpha_text)
{
  Node *node;

  doc = Document::parse("<a>one, 2</a>", &error);
  CHECK_PARSE_ERROR(error);
  root = doc->root();

  node = root->first_child();
  CHECK_TEXT_NODE(node, "one, 2");
}

TEST_F(DocumentParseTest, element_with_mixed_content)
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

TEST_F(DocumentParseTest, comment)
{
  Node *node;

  doc = Document::parse("<a><!--alert--></a>", &error);
  CHECK_PARSE_ERROR(error);

  root = doc->root();

  node = root->first_child();
  ASSERT_EQ(NODE_TYPE_COMMENT, node->type());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
