#include "xpath-value-string.h"

#include "gmock/gmock.h"
#include <iostream>

using testing::HasSubstr;

class XPathValueStringTest : public ::testing::Test {
};

TEST_F(XPathValueStringTest, can_be_a_string)
{
  XPathValueString value("string");
  ASSERT_EQ("string", value.string());
}

TEST_F(XPathValueStringTest, knows_the_native_type_is_string)
{
  XPathValueString value("string");
  ASSERT_TRUE(value.is(XPathValue::Type::String));
}

TEST_F(XPathValueStringTest, converts_empty_string_to_false)
{
  XPathValueString value("");
  ASSERT_EQ(false, value.boolean());
}

TEST_F(XPathValueStringTest, converts_non_empty_string_to_true)
{
  XPathValueString value("string");
  ASSERT_EQ(true, value.boolean());
}

TEST_F(XPathValueStringTest, string_contains_useful_information)
{
  std::stringstream output;
  output << XPathValueString("woof");;
  ASSERT_THAT(output.str(), HasSubstr("String"));
  ASSERT_THAT(output.str(), HasSubstr("woof"));
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
