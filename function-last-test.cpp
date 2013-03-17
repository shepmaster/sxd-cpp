#include "function-last.h"

#include "gmock/gmock.h"
#include "mock-xpath-evaluation-context.h"

#include <iostream>

using testing::NiceMock;
using testing::Return;

class FunctionLastTest : public ::testing::Test {
protected:
  NiceMock<MockEvaluationContext> context;

  std::vector<XPathValue> arguments;
  FunctionLast function;
};

TEST_F(FunctionLastTest, empty_context_returns_zero)
{
  auto result = function.evaluate(context, arguments);

  ASSERT_EQ(XPathValue(0.0), result);
}

TEST_F(FunctionLastTest, returns_number_of_nodes_in_context)
{
  EXPECT_CALL(context, size()).WillRepeatedly(Return(1));

  auto result = function.evaluate(context, arguments);

  ASSERT_EQ(XPathValue(1.0), result);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
