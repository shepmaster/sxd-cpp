#include "xpath-value-string.h"

#include "gmock/gmock.h"
#include <iostream>

using testing::HasSubstr;
using testing::NanSensitiveDoubleEq;

static const double NaN = std::numeric_limits<double>::quiet_NaN();

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

TEST_F(XPathValueStringTest, converts_empty_string_to_NaN)
{
  XPathValueString value("");
  ASSERT_THAT(value.number(), NanSensitiveDoubleEq(NaN));
}

TEST_F(XPathValueStringTest, converts_integral_number)
{
  XPathValueString value("4");
  ASSERT_THAT(value.number(), NanSensitiveDoubleEq(4));
}

TEST_F(XPathValueStringTest, converts_floating_point_number)
{
  XPathValueString value("-3.14");
  ASSERT_THAT(value.number(), NanSensitiveDoubleEq(-3.14));
}

TEST_F(XPathValueStringTest, ignores_leading_space_when_converting_to_number)
{
  XPathValueString value("  1.2");
  ASSERT_THAT(value.number(), NanSensitiveDoubleEq(1.2));
}

TEST_F(XPathValueStringTest, ignores_trailing_space_when_converting_to_number)
{
  XPathValueString value("1.2\t");
  ASSERT_THAT(value.number(), NanSensitiveDoubleEq(1.2));
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
