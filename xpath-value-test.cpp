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

TEST_F(XPathValueTest, knows_the_native_type_is_number)
{
  XPathValue value(3);
  ASSERT_TRUE(value.is(XPathValue::Type::Number));
}

TEST_F(XPathValueTest, knows_the_native_type_is_string)
{
  XPathValue value("string");
  ASSERT_TRUE(value.is(XPathValue::Type::String));
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
