#include "xpath-function-library.h"

#include "xpath-function.h"

#include "gmock/gmock.h"
#include <iostream>

using std::make_shared;

using testing::Return;

class MockFunction : public XPathFunction {
public:
  MOCK_CONST_METHOD0(name, std::string());
  MOCK_CONST_METHOD2(evaluate, XPathValue(const XPathEvaluationContext &context,
                                          const std::vector<XPathValue> arguments));
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
  const std::string function_name = "a-name";

  auto mock_function = make_shared<MockFunction>();
  EXPECT_CALL(*mock_function, name())
    .WillRepeatedly(Return(function_name));

  library.add(mock_function);
  ASSERT_TRUE(library.has_function(function_name));
  auto fn = library.function_for_name(function_name);
  ASSERT_EQ(mock_function, fn);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
