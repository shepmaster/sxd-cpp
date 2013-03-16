#include "xpath-value-boolean.h"

#include "gmock/gmock.h"
#include <iostream>

using testing::HasSubstr;

class XPathValueTest : public ::testing::Test {
};

TEST_F(XPathValueTest, can_be_a_boolean)
{
  XPathValue value(true);
  ASSERT_EQ(true, value.boolean());
}

TEST_F(XPathValueTest, knows_the_native_type_is_boolean)
{
  XPathValue value(false);
  ASSERT_TRUE(value.is(XPathValue::Type::Boolean));
}

TEST_F(XPathValueTest, string_contains_useful_information)
{
  std::stringstream output;
  output << XPathValue(false);
  ASSERT_THAT(output.str(), HasSubstr("Boolean"));
  ASSERT_THAT(output.str(), HasSubstr("false"));
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
