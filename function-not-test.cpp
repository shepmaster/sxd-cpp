#include "function-not.h"

#include "gmock/gmock.h"
#include "mock-xpath-evaluation-context.h"

#include <iostream>

using testing::NiceMock;

class FunctionNotTest : public ::testing::Test {
protected:
  NiceMock<MockEvaluationContext> context;

  std::vector<XPathValue> arguments;
  FunctionNot function;
};

TEST_F(FunctionNotTest, inverts_input_boolean)
{
  arguments.push_back(XPathValue(false));

  auto result = function.evaluate(context, arguments);

  ASSERT_EQ(XPathValue(true), result);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
