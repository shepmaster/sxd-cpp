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

TEST(XPathAcceptance, can_select_nested_child_element)
{
  XPathAxisTestData d;

  XPath xpath = XPathFactory().compile("one/c");
  Nodeset selected_nodes = d.alpha->select_nodes(xpath);

  Nodeset expected_nodes;
  expected_nodes.add(d.c);

  CHECK_EQUAL(expected_nodes, selected_nodes);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
