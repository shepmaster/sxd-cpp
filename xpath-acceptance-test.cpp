#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "xpath-axis-test-data.h"
#include "xpath-factory.h"
#include "xpath.h"

#include "nodeset.h"

TEST_GROUP(XPathAcceptance)
{
};

#include <sstream>

SimpleString
StringFrom(const Nodeset &nodeset)
{
  std::stringstream sstream;
  sstream << nodeset;
  std::string s = sstream.str();
  return SimpleString(s.c_str());
}

TEST(XPathAcceptance, can_select_child_element)
{
  XPathAxisTestData d;

  XPath xpath = XPathFactory().compile("two");
  Nodeset selected_nodes = d.alpha->select_nodes(xpath);

  Nodeset expected_nodes;
  expected_nodes.add(d.two);

  CHECK_EQUAL(expected_nodes, selected_nodes);
}

TEST(XPathAcceptance, can_select_child_wildcard_elements)
{
  XPathAxisTestData d;

  XPath xpath = XPathFactory().compile("*");
  Nodeset selected_nodes = d.alpha->select_nodes(xpath);

  Nodeset expected_nodes;
  expected_nodes.add(d.one);
  expected_nodes.add(d.two);

  CHECK_EQUAL(expected_nodes, selected_nodes);
}

TEST(XPathAcceptance, can_select_grandchild_element)
{
  XPathAxisTestData d;

  XPath xpath = XPathFactory().compile("one/c");
  Nodeset selected_nodes = d.alpha->select_nodes(xpath);

  Nodeset expected_nodes;
  expected_nodes.add(d.c);

  CHECK_EQUAL(expected_nodes, selected_nodes);
}

TEST(XPathAcceptance, can_select_great_grandchild_element)
{
  Document doc;
  Node *one = doc.new_element("one");
  Node *two = doc.new_element("two");
  Node *three = doc.new_element("three");
  Node *four = doc.new_element("four");
  one->append_child(two);
  two->append_child(three);
  three->append_child(four);

  XPath xpath = XPathFactory().compile("two/three/four");
  Nodeset selected_nodes = one->select_nodes(xpath);

  Nodeset expected_nodes;
  expected_nodes.add(four);

  CHECK_EQUAL(expected_nodes, selected_nodes);

  delete one;
}

TEST(XPathAcceptance, can_select_self)
{
  Document doc;
  Node *one = doc.new_element("one");

  XPath xpath = XPathFactory().compile(".");
  Nodeset selected_nodes = one->select_nodes(xpath);

  Nodeset expected_nodes;
  expected_nodes.add(one);

  CHECK_EQUAL(expected_nodes, selected_nodes);

  delete one;
}

TEST(XPathAcceptance, can_select_parent)
{
  Document doc;
  Node *one = doc.new_element("one");
  Node *two = doc.new_element("two");
  one->append_child(two);

  XPath xpath = XPathFactory().compile("..");
  Nodeset selected_nodes = two->select_nodes(xpath);

  Nodeset expected_nodes;
  expected_nodes.add(one);

  CHECK_EQUAL(expected_nodes, selected_nodes);

  delete one;
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
