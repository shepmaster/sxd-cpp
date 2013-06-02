#include "node-test-element.h"

#include "document.h"

#include "gmock/gmock.h"
#include <iostream>

using testing::ElementsAre;

class NodeTestElementTest : public ::testing::Test {
protected:
  Document doc;
  Nodeset result;
};

TEST_F(NodeTestElementTest, selects_nodes_with_matching_names)
{
  auto element = doc.new_element("hello");
  PrefixedName desired_name("hello");
  auto test = NodeTestElement(desired_name);

  test.test(element, result);

  ASSERT_THAT(result, ElementsAre(element));
}

TEST_F(NodeTestElementTest, does_not_select_other_names)
{
  auto element = doc.new_element("hello");
  PrefixedName desired_name("not-hello");
  auto test = NodeTestElement(desired_name);

  test.test(element, result);

  ASSERT_THAT(result, ElementsAre());
}

TEST_F(NodeTestElementTest, does_not_select_when_the_element_does_not_have_a_namespace)
{
  auto element = doc.new_element("hello");
  PrefixedName desired_name("ns", "hello");
  auto test = NodeTestElement(desired_name);

  test.test(element, result);

  ASSERT_THAT(result, ElementsAre());
}

TEST_F(NodeTestElementTest, does_not_select_when_the_prefix_does_not_exist)
{
  auto element = doc.new_element("namespace-uri", "hello");
  PrefixedName desired_name("ns", "hello");
  auto test = NodeTestElement(desired_name);

  test.test(element, result);

  ASSERT_THAT(result, ElementsAre());
}

TEST_F(NodeTestElementTest, does_not_select_when_the_prefix_does_not_match_the_namespace)
{
  auto element = doc.new_element("namespace-uri", "hello");
  element->set_namespace_prefix("ns", "another-uri");
  PrefixedName desired_name("ns", "hello");
  auto test = NodeTestElement(desired_name);

  test.test(element, result);

  ASSERT_THAT(result, ElementsAre());
}

TEST_F(NodeTestElementTest, supports_a_wildcard_match)
{
  auto element = doc.new_element("hello");
  PrefixedName desired_name("*");
  auto test = NodeTestElement(desired_name);

  test.test(element, result);

  ASSERT_THAT(result, ElementsAre(element));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
