#include "expression-function.h"

#include "xpath-parsing-exceptions.h"

#include "gmock/gmock.h"
#include "mock-xpath-expression.h"
#include "mock-xpath-function.h"
#include "mock-xpath-evaluation-context.h"

#include <iostream>

using std::make_shared;

using testing::DefaultValue;
using testing::NiceMock;
using testing::Ref;
using testing::Return;
using testing::_;

class ExpressionFunctionTest : public ::testing::Test {
protected:
  std::shared_ptr<MockFunction> function = make_shared<NiceMock<MockFunction>>();
  std::vector<std::shared_ptr<XPathExpression>> arguments;

  NiceMock<MockEvaluationContext> context;

  void SetUp() {
    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }
};

TEST_F(ExpressionFunctionTest, evaluates_input_arguments)
{
  EXPECT_CALL(context, has_function("foo")).WillRepeatedly(Return(true));
  EXPECT_CALL(context, function_for_name("foo")).WillRepeatedly(Return(function));

  auto argument = make_shared<MockExpression>();
  EXPECT_CALL(*argument, evaluate(Ref(context)));
  arguments.push_back(argument);

  ExpressionFunction expression("foo", arguments);

  expression.evaluate(context);
}

TEST_F(ExpressionFunctionTest, unknown_function_is_reported_as_an_error)
{
  ExpressionFunction expression("unknown-function", arguments);

  ASSERT_THROW(expression.evaluate(context), UnknownXPathFunctionException);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
