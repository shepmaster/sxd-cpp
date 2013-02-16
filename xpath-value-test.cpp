#include "xpath-value.h"

#include "gmock/gmock.h"
#include <iostream>

class XPathValueTest : public ::testing::Test {
};

TEST_F(XPathValueTest, can_be_numeric)
{
  XPathValue value(42.42);
  ASSERT_EQ(42.42, value.number());
}

TEST_F(XPathValueTest, can_be_a_string)
{
  XPathValue value("string");
  ASSERT_EQ("string", value.string());
}

TEST_F(XPathValueTest, can_be_a_boolean)
{
  XPathValue value(true);
  ASSERT_EQ(true, value.boolean());
}

TEST_F(XPathValueTest, knows_the_native_type_is_number)
{
  XPathValue value(3.0);
  ASSERT_TRUE(value.is(XPathValue::Type::Number));
}

TEST_F(XPathValueTest, knows_the_native_type_is_string)
{
  XPathValue value("string");
  ASSERT_TRUE(value.is(XPathValue::Type::String));
}

TEST_F(XPathValueTest, knows_the_native_type_is_boolean)
{
  XPathValue value(false);
  ASSERT_TRUE(value.is(XPathValue::Type::Boolean));
}

TEST_F(XPathValueTest, converts_empty_string_to_false)
{
  XPathValue value("");
  ASSERT_EQ(false, value.boolean());
}

TEST_F(XPathValueTest, converts_non_empty_string_to_true)
{
  XPathValue value("string");
  ASSERT_EQ(true, value.boolean());
}

TEST_F(XPathValueTest, converts_positive_zero_to_false)
{
  XPathValue value(0.0);
  ASSERT_EQ(false, value.boolean());
}

TEST_F(XPathValueTest, converts_negative_zero_to_false)
{
  XPathValue value(-0.0);
  ASSERT_EQ(false, value.boolean());
}

TEST_F(XPathValueTest, converts_nan_to_false)
{
  auto NaN = std::numeric_limits<double>::quiet_NaN();
  XPathValue value(NaN);
  ASSERT_EQ(false, value.boolean());
}

TEST_F(XPathValueTest, converts_other_number_to_true)
{
  XPathValue value(42.42);
  ASSERT_EQ(true, value.boolean());
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
