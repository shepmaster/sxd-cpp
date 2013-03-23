#include "xpath-core-function-library.h"

#include "gmock/gmock.h"
#include <iostream>

using testing::ValuesIn;

class XPathCoreFunctionLibraryTest : public ::testing::Test,
                                     public ::testing::WithParamInterface<std::string>
{
protected:
  XPathFunctionLibrary library;

  void SetUp() {
    XPathCoreFunctionLibrary::register_functions(library);
  }
};

TEST_P(XPathCoreFunctionLibraryTest, function_exists)
{
  ASSERT_TRUE(library.has_function(GetParam()));
}

std::string functions[] = {
  "last",
  "position",
  "not",
  "true",
  "false",
};

INSTANTIATE_TEST_CASE_P(FunctionExists,
                        XPathCoreFunctionLibraryTest,
                        ValuesIn(functions));

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
