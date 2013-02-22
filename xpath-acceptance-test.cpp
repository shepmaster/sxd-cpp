#include "nodeset.h"

#include "xpath-factory.h"
#include "document.h"

#include "gmock/gmock.h"
#include <iostream>

using testing::ElementsAre;

class XPathAcceptanceTest : public ::testing::Test {
protected:
  Document doc;

  XPath
  compile(std::string xpath) {
    return XPathFactory().compile(xpath);
  }

  Node *
  add_child(Node *top_node, std::string name) {
    Node *n = doc.new_element(name);
    top_node->append_child(n);
    return n;
  }

  Node *
  add_text_node(Node *parent, std::string text) {
    Node *n = doc.new_text_node(text);
    parent->append_child(n);
    return n;
  }

  Node *
  set_attribute(Node *parent, std::string name, std::string value) {
    return dynamic_cast<Element *>(parent)->set_attribute(name, value);
  }
};

TEST_F(XPathAcceptanceTest, name_selects_child_element)
{
  Node *one = doc.new_element("one");
  Node *two = add_child(one, "two");

  XPath xpath = compile("two");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(two));
}

TEST_F(XPathAcceptanceTest, asterisk_selects_all_children_elements)
{
  Node *top = doc.new_element("top");
  Node *child1 = add_child(top, "child1");
  Node *child2 = add_child(top, "child2");

  XPath xpath = compile("*");
  Nodeset selected_nodes = top->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(child1, child2));
}

TEST_F(XPathAcceptanceTest, slash_selects_grandchild_element)
{
  Node *one = doc.new_element("one");
  Node *two = add_child(one, "two");
  Node *three = add_child(two, "three");

  XPath xpath = compile("two/three");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(three));
}

TEST_F(XPathAcceptanceTest, multiple_slashes_select_great_grandchild_element)
{
  Node *one = doc.new_element("one");
  Node *two = add_child(one, "two");
  Node *three = add_child(two, "three");
  Node *four = add_child(three, "four");

  XPath xpath = compile("two/three/four");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(four));
}

TEST_F(XPathAcceptanceTest, dot_abbreviation_selects_self)
{
  Node *one = doc.new_element("one");

  XPath xpath = compile(".");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));
}

TEST_F(XPathAcceptanceTest, self_axis_selects_self)
{
  Node *one = doc.new_element("one");

  XPath xpath = compile("self::one");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));
}

TEST_F(XPathAcceptanceTest, self_axis_without_matching_element_name_selects_nothing)
{
  Node *one = doc.new_element("one");

  XPath xpath = compile("self::two");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_TRUE(selected_nodes.empty());
}

TEST_F(XPathAcceptanceTest, double_dot_abbreviation_selects_parent)
{
  Node *one = doc.new_element("one");
  Node *two = add_child(one, "two");

  XPath xpath = compile("..");
  Nodeset selected_nodes = two->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));
}

TEST_F(XPathAcceptanceTest, parent_axis_selects_parent)
{
  Node *one = doc.new_element("one");
  Node *two = add_child(one, "two");

  XPath xpath = compile("parent::one");
  Nodeset selected_nodes = two->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));
}

TEST_F(XPathAcceptanceTest, two_double_dots_selects_grandparent)
{
  Node *one = doc.new_element("one");
  Node *two = add_child(one, "two");
  Node *three = add_child(two, "three");

  XPath xpath = compile("../..");
  Nodeset selected_nodes = three->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));
}

TEST_F(XPathAcceptanceTest, selector_with_same_name_as_an_axis_selects_element)
{
  Node *one = doc.new_element("one");
  Node *self = add_child(one, "self");

  XPath xpath = compile("self");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(self));
}

TEST_F(XPathAcceptanceTest, descendant_axis_selects_all_children)
{
  Node *parent = doc.new_element("parent");
  Node *child = add_child(parent, "child");
  Node *grandchild = add_child(child, "grandchild");

  XPath xpath = compile("descendant::*");
  Nodeset selected_nodes = parent->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(child, grandchild));
}

TEST_F(XPathAcceptanceTest, double_slash_abbreviation_selects_self_and_all_children)
{
  Node *parent = doc.new_element("yup");
  Node *child = add_child(parent, "nope");
  Node *grandchild = add_child(child, "yup");

  XPath xpath = compile(".//self::yup");
  Nodeset selected_nodes = parent->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(parent, grandchild));
}

TEST_F(XPathAcceptanceTest, text_node_node_test_selects_text_nodes)
{
  Node *parent = doc.new_element("element");
  Node *text = add_text_node(parent, "some text");

  XPath xpath = compile("text()");
  Nodeset selected_nodes = parent->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(text));
}

TEST_F(XPathAcceptanceTest, at_sign_abbreviation_selects_attributes)
{
  Node *element = doc.new_element("element");
  Node *attribute = set_attribute(element, "name", "value");

  XPath xpath = compile("@name");
  Nodeset selected_nodes = element->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(attribute));
}

TEST_F(XPathAcceptanceTest, numeric_predicate_selects_nth_child)
{
  Node *element = doc.new_element("element");
  add_child(element, "one");
  Node *two = add_child(element, "two");

  XPath xpath = compile("*[2]");
  Nodeset selected_nodes = element->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(two));
}

TEST_F(XPathAcceptanceTest, string_predicate_selects_all_children)
{
  Node *element = doc.new_element("element");
  Node *one = add_child(element, "one");
  Node *two = add_child(element, "two");

  XPath xpath = compile("*['hello']");
  Nodeset selected_nodes = element->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one, two));
}

TEST_F(XPathAcceptanceTest, true_boolean_predicate_selects_all_children)
{
  Node *element = doc.new_element("element");
  Node *one = add_child(element, "one");
  Node *two = add_child(element, "two");

  XPath xpath = compile("*[true()]");
  Nodeset selected_nodes = element->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one, two));
}

TEST_F(XPathAcceptanceTest, predicate_functions_can_accept_one_argument)
{
  Node *element = doc.new_element("element");
  add_child(element, "one");
  add_child(element, "two");

  XPath xpath = compile("*[not(true())]");
  Nodeset selected_nodes = element->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre());
}

TEST_F(XPathAcceptanceTest, invalid_axis_throws_exception)
{
  ASSERT_THROW(compile("bogus::*"), InvalidXPathAxisException);
}

TEST_F(XPathAcceptanceTest, invalid_node_test_throws_exception)
{
  ASSERT_THROW(compile("bogus()"), InvalidNodeTestException);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
