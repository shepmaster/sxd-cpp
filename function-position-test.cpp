#include "function-position.h"

#include "gmock/gmock.h"
#include "mock-xpath-evaluation-context.h"

#include <iostream>

using testing::NiceMock;
using testing::Return;

class FunctionPositionTest : public ::testing::Test {
protected:
  NiceMock<MockEvaluationContext> context;
  std::vector<XPathValue> arguments;

  FunctionPosition function;
};

TEST_F(FunctionPositionTest, delegates_to_the_context)
{
  EXPECT_CALL(context, position()).WillRepeatedly(Return(3));

  auto result = function.evaluate(context, arguments);

  ASSERT_EQ(3.0, result.number());
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
