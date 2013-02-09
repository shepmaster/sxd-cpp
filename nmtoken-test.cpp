#include "nmtoken.h"

#include "gmock/gmock.h"
#include <iostream>

class NMTokenTest : public ::testing::Test
{};

TEST_F(NMTokenTest, new_nmtoken)
{
  NMToken nm("xs:boolean");

  ASSERT_STREQ("boolean", nm.name());
  ASSERT_STREQ("xs", nm.ns());
}

TEST_F(NMTokenTest, new_name_only)
{
  NMToken nm("string");

  ASSERT_STREQ("string", nm.name());
  ASSERT_EQ(nullptr, nm.ns());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
