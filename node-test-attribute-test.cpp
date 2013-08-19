#include "node-test-attribute.h"

#include "document.h"

#include "gmock/gmock.h"
#include "mock-xpath-evaluation-context.h"

#include <iostream>

using testing::ElementsAre;
using testing::NiceMock;
using testing::Return;

class NodeTestAttributeTest : public ::testing::Test {
protected:
  Document doc;
  Element *element = doc.new_element("element");
  NiceMock<MockEvaluationContext> context;
  Nodeset result;
};

TEST_F(NodeTestAttributeTest, selects_attributes_with_matching_names)
{
  auto attribute = element->set_attribute("hello", "world");
  auto test = NodeTestAttribute("hello");
  EXPECT_CALL(context, node()).WillRepeatedly(Return(attribute));

  test.test(context, result);

  ASSERT_THAT(result, ElementsAre(attribute));
}

TEST_F(NodeTestAttributeTest, does_not_select_elements)
{
  auto test = NodeTestAttribute("element");
  EXPECT_CALL(context, node()).WillRepeatedly(Return(element));

  test.test(context, result);

  ASSERT_THAT(result, ElementsAre());
}

TEST_F(NodeTestAttributeTest, supports_a_wildcard_match)
{
  auto attribute = element->set_attribute("hello", "world");
  auto test = NodeTestAttribute("*");
  EXPECT_CALL(context, node()).WillRepeatedly(Return(attribute));

  test.test(context, result);

  ASSERT_THAT(result, ElementsAre(attribute));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
