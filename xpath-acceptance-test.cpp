#include "nodeset.h"

#include "xpath-factory.h"
#include "xpath-parsing-exceptions.h"
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

  Element *
  add_child(Node *top_node, std::string name) {
    auto n = doc.new_element(name);
    top_node->append_child(n);
    return n;
  }

  TextNode *
  add_text_node(Node *parent, std::string text) {
    auto tn = doc.new_text_node(text);
    parent->append_child(tn);
    return tn;
  }

  Attribute *
  set_attribute(Element *parent, std::string name, std::string value) {
    return parent->set_attribute(name, value);
  }
};

TEST_F(XPathAcceptanceTest, name_selects_child_element)
{
  Element *one = doc.new_element("one");
  Element *two = add_child(one, "two");

  XPath xpath = compile("two");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(two));
}

TEST_F(XPathAcceptanceTest, asterisk_selects_all_children_elements)
{
  Element *top = doc.new_element("top");
  Element *child1 = add_child(top, "child1");
  Element *child2 = add_child(top, "child2");

  XPath xpath = compile("*");
  Nodeset selected_nodes = top->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(child1, child2));
}

TEST_F(XPathAcceptanceTest, slash_selects_grandchild_element)
{
  Element *one = doc.new_element("one");
  Element *two = add_child(one, "two");
  Element *three = add_child(two, "three");

  XPath xpath = compile("two/three");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(three));
}

TEST_F(XPathAcceptanceTest, multiple_slashes_select_great_grandchild_element)
{
  Element *one = doc.new_element("one");
  Element *two = add_child(one, "two");
  Element *three = add_child(two, "three");
  Element *four = add_child(three, "four");

  XPath xpath = compile("two/three/four");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(four));
}

TEST_F(XPathAcceptanceTest, dot_abbreviation_selects_self)
{
  Element *one = doc.new_element("one");

  XPath xpath = compile(".");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));
}

TEST_F(XPathAcceptanceTest, self_axis_selects_self)
{
  Element *one = doc.new_element("one");

  XPath xpath = compile("self::one");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));
}

TEST_F(XPathAcceptanceTest, self_axis_without_matching_element_name_selects_nothing)
{
  Element *one = doc.new_element("one");

  XPath xpath = compile("self::two");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_TRUE(selected_nodes.empty());
}

TEST_F(XPathAcceptanceTest, double_dot_abbreviation_selects_parent)
{
  Element *one = doc.new_element("one");
  Element *two = add_child(one, "two");

  XPath xpath = compile("..");
  Nodeset selected_nodes = two->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));
}

TEST_F(XPathAcceptanceTest, parent_axis_selects_parent)
{
  Element *one = doc.new_element("one");
  Element *two = add_child(one, "two");

  XPath xpath = compile("parent::one");
  Nodeset selected_nodes = two->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));
}

TEST_F(XPathAcceptanceTest, two_double_dots_selects_grandparent)
{
  Element *one = doc.new_element("one");
  Element *two = add_child(one, "two");
  Element *three = add_child(two, "three");

  XPath xpath = compile("../..");
  Nodeset selected_nodes = three->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));
}

TEST_F(XPathAcceptanceTest, absolute_path_returns_to_the_root)
{
  Element *one = doc.new_element("one");
  Element *two = add_child(one, "two");
  Element *three = add_child(two, "three");

  doc.root()->append_child(one);

  XPath xpath = compile("/*");
  Nodeset selected_nodes = three->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one));
}

TEST_F(XPathAcceptanceTest, absolute_double_slash_starts_from_the_root)
{
  Element *one = doc.new_element("one");
  Element *two = add_child(one, "two");
  Element *three = add_child(two, "three");

  doc.root()->append_child(one);

  XPath xpath = compile("//*");
  Nodeset selected_nodes = three->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one, two, three));
}

TEST_F(XPathAcceptanceTest, selector_with_same_name_as_an_axis_selects_element)
{
  Element *one = doc.new_element("one");
  Element *self = add_child(one, "self");

  XPath xpath = compile("self");
  Nodeset selected_nodes = one->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(self));
}

TEST_F(XPathAcceptanceTest, descendant_axis_selects_all_children)
{
  Element *parent = doc.new_element("parent");
  Element *child = add_child(parent, "child");
  Element *grandchild = add_child(child, "grandchild");

  XPath xpath = compile("descendant::*");
  Nodeset selected_nodes = parent->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(child, grandchild));
}

TEST_F(XPathAcceptanceTest, double_slash_abbreviation_selects_self_and_all_children)
{
  Element *parent = doc.new_element("yup");
  Element *child = add_child(parent, "nope");
  Element *grandchild = add_child(child, "yup");

  XPath xpath = compile(".//self::yup");
  Nodeset selected_nodes = parent->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(parent, grandchild));
}

TEST_F(XPathAcceptanceTest, text_node_node_test_selects_text_nodes)
{
  Element *parent = doc.new_element("element");
  TextNode *text = add_text_node(parent, "some text");

  XPath xpath = compile("text()");
  Nodeset selected_nodes = parent->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(text));
}

TEST_F(XPathAcceptanceTest, at_sign_abbreviation_selects_attributes)
{
  Element *element = doc.new_element("element");
  Attribute *attribute = set_attribute(element, "name", "value");

  XPath xpath = compile("@name");
  Nodeset selected_nodes = element->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(attribute));
}

TEST_F(XPathAcceptanceTest, numeric_predicate_selects_nth_child)
{
  Element *element = doc.new_element("element");
  add_child(element, "one");
  Element *two = add_child(element, "two");

  XPath xpath = compile("*[2]");
  Nodeset selected_nodes = element->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(two));
}

TEST_F(XPathAcceptanceTest, string_predicate_selects_all_children)
{
  Element *element = doc.new_element("element");
  Element *one = add_child(element, "one");
  Element *two = add_child(element, "two");

  XPath xpath = compile("*['hello']");
  Nodeset selected_nodes = element->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one, two));
}

TEST_F(XPathAcceptanceTest, true_boolean_predicate_selects_all_children)
{
  Element *element = doc.new_element("element");
  Element *one = add_child(element, "one");
  Element *two = add_child(element, "two");

  XPath xpath = compile("*[true()]");
  Nodeset selected_nodes = element->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(one, two));
}

TEST_F(XPathAcceptanceTest, predicate_functions_can_accept_one_argument)
{
  Element *element = doc.new_element("element");
  add_child(element, "one");
  add_child(element, "two");

  XPath xpath = compile("*[not(true())]");
  Nodeset selected_nodes = element->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre());
}

TEST_F(XPathAcceptanceTest, additive_math_operations)
{
  XPath xpath = compile("1 - 2 + 3");

  ASSERT_DOUBLE_EQ(2.0, xpath.number());
}

TEST_F(XPathAcceptanceTest, multiplicitive_math_operations)
{
  XPath xpath = compile("5 mod 2 div 3 * 2");

  ASSERT_DOUBLE_EQ(2/3.0, xpath.number());
}

TEST_F(XPathAcceptanceTest, unary_negation)
{
  XPath xpath = compile("2 - -2");

  ASSERT_DOUBLE_EQ(4.0, xpath.number());
}

TEST_F(XPathAcceptanceTest, union_of_selections)
{
  Element *parent = doc.new_element("parent");
  Element *first = add_child(parent, "first");
  Element *second = add_child(parent, "second");

  XPath xpath = compile("first|second");
  Nodeset selected_nodes = parent->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(first, second));
}

TEST_F(XPathAcceptanceTest, namespaced_nodes)
{
  Element *parent = doc.new_element("parent");
  Element *namespaced_node = doc.new_element("namespace", "child");
  parent->set_namespace_prefix("ns", "namespace");
  parent->append_child(namespaced_node);

  XPath xpath = compile("ns:child");
  Nodeset selected_nodes = parent->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre(namespaced_node));
}

TEST_F(XPathAcceptanceTest, selecting_the_wrong_namespace_returns_nothing)
{
  Element *parent = doc.new_element("parent");
  Element *namespaced_node = doc.new_element("namespace", "child");
  namespaced_node->set_namespace_prefix("ns1", "namespace");
  namespaced_node->set_namespace_prefix("ns2", "another-namespace");
  parent->append_child(namespaced_node);

  XPath xpath = compile("ns2:child");
  Nodeset selected_nodes = parent->select_nodes(xpath);

  ASSERT_THAT(selected_nodes, ElementsAre());
}

TEST_F(XPathAcceptanceTest, location_paths_apply_to_variables)
{
  Element *parent = doc.new_element("parent");
  Element *child = add_child(parent, "child");

  XPath xpath = compile("$variable/child");

  Nodeset variable_value;
  variable_value.add(parent);
  xpath.bind_variable("variable", variable_value);

  ASSERT_THAT(xpath.nodeset(), ElementsAre(child));
}

TEST_F(XPathAcceptanceTest, boolean_logic)
{
  XPath xpath = compile("true() and false()");

  ASSERT_EQ(false, xpath.boolean());
}

TEST_F(XPathAcceptanceTest, equality)
{
  XPath xpath = compile("1.3 != '1.3'");

  ASSERT_EQ(false, xpath.boolean());
}

TEST_F(XPathAcceptanceTest, relational)
{
  XPath xpath = compile("1.0 >= 2.1");

  ASSERT_EQ(false, xpath.boolean());
}

TEST_F(XPathAcceptanceTest, variables)
{
  XPath xpath = compile("$variable");

  xpath.bind_variable("variable", 5.2);
  ASSERT_DOUBLE_EQ(5.2, xpath.number());
}

TEST_F(XPathAcceptanceTest, extra_junk_at_end_throws_exception)
{
  ASSERT_THROW(compile("1 2"), ExtraUnparsedTokensException);
}

TEST_F(XPathAcceptanceTest, invalid_axis_throws_exception)
{
  ASSERT_THROW(compile("bogus::*"), InvalidXPathAxisException);
}

TEST_F(XPathAcceptanceTest, invalid_function_throws_exception)
{
  XPath xpath = compile("bogus()");
  ASSERT_THROW(xpath.number(), UnknownXPathFunctionException);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
