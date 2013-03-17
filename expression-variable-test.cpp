#include "expression-variable.h"

#include "xpath-parsing-exceptions.h"

#include "gmock/gmock.h"
#include "mock-xpath-evaluation-context.h"

#include <iostream>

using std::make_shared;
using std::shared_ptr;

using testing::DefaultValue;
using testing::NiceMock;
using testing::Ref;
using testing::Return;
using testing::_;

class ExpressionVariableTest : public ::testing::Test {
protected:
  NiceMock<MockEvaluationContext> context;

  void SetUp() {
    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }
};

TEST_F(ExpressionVariableTest, requests_variable_from_bindings)
{
  EXPECT_CALL(context, variable_for_name("foo"));

  ExpressionVariable expression("foo");

  expression.evaluate(context);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
