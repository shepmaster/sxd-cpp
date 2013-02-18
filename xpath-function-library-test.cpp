#include "xpath-function-library.h"

#include "gmock/gmock.h"
#include <iostream>

using std::make_shared;

using testing::Return;

class MockFunction : public XPathFunction {
public:
  MOCK_CONST_METHOD0(name, std::string());
  MOCK_CONST_METHOD0(evaluate, XPathValue());
};

class XPathFunctionLibraryTest : public ::testing::Test {
protected:
  XPathFunctionLibrary library;
};

TEST_F(XPathFunctionLibraryTest, starts_with_no_functions)
{
  auto fn = library.function_for_name("anything");
  ASSERT_EQ(nullptr, fn);
}

TEST_F(XPathFunctionLibraryTest, can_add_a_function)
{
  auto mock_function = make_shared<MockFunction>();
  EXPECT_CALL(*mock_function, name())
    .WillRepeatedly(Return("a-name"));

  library.add(mock_function);
  auto fn = library.function_for_name("a-name");
  ASSERT_EQ(mock_function, fn);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
