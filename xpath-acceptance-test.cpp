#include "nodeset.h"

#include "xpath.h"
#include "xpath-factory.h"
#include "xpath-axis-test-data.h"

#include "gmock/gmock.h"
#include <iostream>

using testing::ElementsAre;

class XPathAcceptanceTest : public ::testing::Test {
protected:
  Document doc;
};

Node *
add_child(Node *top_node, std::string name) {
  Document *doc = top_node->document();
  Node *n = doc->new_element(name.c_str());
  top_node->append_child(n);
  return n;
}

TEST_F(XPathAcceptanceTest, can_select_child_element)
{
  XPathAxisTestData d;

  XPath xpath = XPathFactory().compile("two");
  Nodeset selected_nodes = d.alpha->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(d.two));
}

TEST_F(XPathAcceptanceTest, can_select_child_wildcard_elements)
{
  XPathAxisTestData d;

  XPath xpath = XPathFactory().compile("*");
  Nodeset selected_nodes = d.alpha->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(d.one, d.two));
}

TEST_F(XPathAcceptanceTest, can_select_grandchild_element)
{
  XPathAxisTestData d;

  XPath xpath = XPathFactory().compile("one/c");
  Nodeset selected_nodes = d.alpha->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(d.c));
}

TEST_F(XPathAcceptanceTest, can_select_great_grandchild_element)
{
  Node *one = doc.new_element("one");
  Node *two = add_child(one, "two");
  Node *three = add_child(two, "three");
  Node *four = add_child(three, "four");

  XPath xpath = XPathFactory().compile("two/three/four");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(four));

  delete one;
}

TEST_F(XPathAcceptanceTest, can_select_self_using_abbreviated_step)
{
  Node *one = doc.new_element("one");

  XPath xpath = XPathFactory().compile(".");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));

  delete one;
}

TEST_F(XPathAcceptanceTest, can_select_self_using_axis)
{
  Node *one = doc.new_element("one");

  XPath xpath = XPathFactory().compile("self::one");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));

  delete one;
}

TEST_F(XPathAcceptanceTest, self_axis_without_matching_element_name_has_no_results)
{
  Node *one = doc.new_element("one");

  XPath xpath = XPathFactory().compile("self::two");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_TRUE(selected_nodes.empty());

  delete one;
}

TEST_F(XPathAcceptanceTest, can_select_parent_using_abbreviated_step)
{
  Node *one = doc.new_element("one");
  Node *two = add_child(one, "two");

  XPath xpath = XPathFactory().compile("..");
  Nodeset selected_nodes = two->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));

  delete one;
}

TEST_F(XPathAcceptanceTest, can_select_parent_using_axis)
{
  Node *one = doc.new_element("one");
  Node *two = add_child(one, "two");

  XPath xpath = XPathFactory().compile("parent::one");
  Nodeset selected_nodes = two->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));

  delete one;
}

TEST_F(XPathAcceptanceTest, can_select_grandparent)
{
  Node *one = doc.new_element("one");
  Node *two = add_child(one, "two");
  Node *three = add_child(two, "three");

  XPath xpath = XPathFactory().compile("../..");
  Nodeset selected_nodes = three->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));

  delete one;
}

TEST_F(XPathAcceptanceTest, can_select_node_with_same_name_as_axis)
{
  Node *one = doc.new_element("one");
  Node *self = add_child(one, "self");

  XPath xpath = XPathFactory().compile("self");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(self));

  delete one;
}

TEST_F(XPathAcceptanceTest, descendant_selects_all_children)
{
  Node *parent = doc.new_element("parent");
  Node *child = add_child(parent, "child");
  Node *grandchild = add_child(child, "grandchild");

  XPath xpath = XPathFactory().compile("descendant::*");
  Nodeset selected_nodes = parent->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(child, grandchild));

  delete parent;
}

TEST_F(XPathAcceptanceTest, double_slash_selects_self_and_all_children)
{
  Node *parent = doc.new_element("yup");
  Node *child = add_child(parent, "nope");
  Node *grandchild = add_child(child, "yup");

  XPath xpath = XPathFactory().compile(".//self::yup");
  Nodeset selected_nodes = parent->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(parent, grandchild));

  delete parent;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
