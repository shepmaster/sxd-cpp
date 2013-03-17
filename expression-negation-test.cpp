#include "expression-negation.h"

#include "gmock/gmock.h"
#include "mock-xpath-expression.h"
#include "expression-test-support.h"

#include <iostream>

using std::shared_ptr;
using std::make_shared;

using testing::DefaultValue;
using testing::NiceMock;
using testing::Ref;
using testing::Return;
using testing::_;

class ExpressionNegationTest : public ::testing::Test {
protected:
  shared_ptr<MockExpression> child_expression = make_shared<NiceMock<MockExpression>>();

  ExpressionTestSupport support;
  XPathEvaluationContext context = support.context();

  void SetUp() {
    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }
};

TEST_F(ExpressionNegationTest, evaluates_both_arguments)
{
  ExpressionNegation expression(child_expression);

  EXPECT_CALL(*child_expression, evaluate(Ref(context)));

  expression.evaluate(context);
}

TEST_F(ExpressionNegationTest, negates_argument)
{
  ExpressionNegation expression(child_expression);

  EXPECT_CALL(*child_expression, evaluate(_)).WillRepeatedly(Return(55.0));

  auto result = expression.evaluate(context);

  ASSERT_DOUBLE_EQ(-55.0, result.number());
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
