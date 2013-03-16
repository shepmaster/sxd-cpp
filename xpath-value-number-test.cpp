#include "xpath-value-number.h"

#include "gmock/gmock.h"
#include <iostream>

using testing::HasSubstr;

class XPathValueNumberTest : public ::testing::Test {
};

TEST_F(XPathValueNumberTest, can_be_numeric)
{
  XPathValueNumber value(42.42);
  ASSERT_EQ(42.42, value.number());
}

TEST_F(XPathValueNumberTest, knows_the_native_type_is_number)
{
  XPathValueNumber value(3.0);
  ASSERT_TRUE(value.is(XPathValue::Type::Number));
}

TEST_F(XPathValueNumberTest, converts_positive_zero_to_false)
{
  XPathValueNumber value(0.0);
  ASSERT_EQ(false, value.boolean());
}

TEST_F(XPathValueNumberTest, converts_negative_zero_to_false)
{
  XPathValueNumber value(-0.0);
  ASSERT_EQ(false, value.boolean());
}

TEST_F(XPathValueNumberTest, converts_nan_to_false)
{
  auto NaN = std::numeric_limits<double>::quiet_NaN();
  XPathValueNumber value(NaN);
  ASSERT_EQ(false, value.boolean());
}

TEST_F(XPathValueNumberTest, converts_other_number_to_true)
{
  XPathValueNumber value(42.42);
  ASSERT_EQ(true, value.boolean());
}

TEST_F(XPathValueNumberTest, string_contains_useful_information)
{
  std::stringstream output;
  XPathValueNumber value(54.3);
  output << value;
  ASSERT_THAT(output.str(), HasSubstr("Number"));
  ASSERT_THAT(output.str(), HasSubstr("54.3"));
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
