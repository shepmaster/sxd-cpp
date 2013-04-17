#include "qname.h"

#include "gmock/gmock.h"

#include <iostream>

class QNameTest : public ::testing::Test {
protected:
};

TEST_F(QNameTest, same_namespace_and_name_are_equal)
{
  QName a("ns", "name");
  QName b("ns", "name");

  ASSERT_EQ(a, b);
}

TEST_F(QNameTest, same_namespace_but_different_names_are_not_equal)
{
  QName a("ns", "name1");
  QName b("ns", "name2");

  ASSERT_NE(a, b);
}

TEST_F(QNameTest, same_name_but_different_namespaces_are_not_equal)
{
  QName a("ns1", "name");
  QName b("ns2", "name");

  ASSERT_NE(a, b);
}

TEST_F(QNameTest, no_namespace_and_same_name_are_equal)
{
  QName a("name");
  QName b("name");

  ASSERT_EQ(a, b);
}

TEST_F(QNameTest, no_namespace_but_different_names_are_not_equal)
{
  QName a("name1");
  QName b("name2");

  ASSERT_NE(a, b);
}

TEST_F(QNameTest, no_namespace_and_empty_namespace_are_not_equal)
{
  QName a("name");
  QName b("", "name");

  ASSERT_NE(a, b);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
