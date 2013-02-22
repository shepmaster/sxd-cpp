#include "xpath-core-function-library.h"

#include "gmock/gmock.h"
#include <iostream>

class XPathCoreFunctionLibraryTest : public ::testing::Test {
protected:
  XPathFunctionLibrary library;

  void SetUp() {
    XPathCoreFunctionLibrary::register_functions(library);
  }
};

TEST_F(XPathCoreFunctionLibraryTest, last)
{
  ASSERT_TRUE(library.has_function("last"));
}

TEST_F(XPathCoreFunctionLibraryTest, not)
{
  ASSERT_TRUE(library.has_function("not"));
}

TEST_F(XPathCoreFunctionLibraryTest, true)
{
  ASSERT_TRUE(library.has_function("true"));
}

TEST_F(XPathCoreFunctionLibraryTest, false)
{
  ASSERT_TRUE(library.has_function("false"));
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
