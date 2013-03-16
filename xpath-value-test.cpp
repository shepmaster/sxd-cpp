#include "xpath-value.h"

#include "gmock/gmock.h"
#include <iostream>

class XPathValueTest : public ::testing::Test {
};

TEST_F(XPathValueTest, has_number_convenience_method)
{
  XPathValue value(42.42);
  ASSERT_EQ(42.42, value.number());
  ASSERT_TRUE(value.is(XPathValue::Type::Number));
}

TEST_F(XPathValueTest, has_string_convenience_method)
{
  XPathValue value("string");
  ASSERT_EQ("string", value.string());
  ASSERT_TRUE(value.is(XPathValue::Type::String));
}

TEST_F(XPathValueTest, has_boolean_convenience_method)
{
  XPathValue value(true);
  ASSERT_EQ(true, value.boolean());
  ASSERT_TRUE(value.is(XPathValue::Type::Boolean));
}

TEST_F(XPathValueTest, has_nodeset_convenience_method)
{
  Nodeset nodes;
  XPathValue value(nodes);
  ASSERT_EQ(nodes, value.nodeset());
  ASSERT_TRUE(value.is(XPathValue::Type::Nodeset));
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
