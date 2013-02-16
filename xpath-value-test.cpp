#include "xpath-value.h"

#include "gmock/gmock.h"
#include <iostream>

class XPathValueTest : public ::testing::Test {
};

TEST_F(XPathValueTest, value_can_be_numeric)
{
  XPathValue value(42.42);
  ASSERT_EQ(42.42, value.number());
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
