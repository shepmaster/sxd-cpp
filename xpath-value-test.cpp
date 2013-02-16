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

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
