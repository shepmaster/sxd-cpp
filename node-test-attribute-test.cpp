#include "node-test-attribute.h"

#include "document.h"

#include "gmock/gmock.h"
#include <iostream>

using testing::ElementsAre;

class NodeTestAttributeTest : public ::testing::Test {
protected:
  Document doc;
  Element *element = doc.new_element("element");
  Nodeset result;
};

TEST_F(NodeTestAttributeTest, selects_attributes_with_matching_names)
{
  auto attribute = element->set_attribute("hello", "world");
  auto test = NodeTestAttribute("hello");

  test.test(attribute, result);

  ASSERT_THAT(result, ElementsAre(attribute));
}

TEST_F(NodeTestAttributeTest, does_not_select_elements)
{
  auto test = NodeTestAttribute("element");

  test.test(element, result);

  ASSERT_THAT(result, ElementsAre());
}

TEST_F(NodeTestAttributeTest, supports_a_wildcard_match)
{
  auto attribute = element->set_attribute("hello", "world");
  auto test = NodeTestAttribute("*");

  test.test(attribute, result);

  ASSERT_THAT(result, ElementsAre(attribute));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
