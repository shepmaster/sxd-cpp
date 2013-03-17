#include "expression-relational.h"

#include "gmock/gmock.h"
#include "mock-xpath-expression.h"
#include "mock-xpath-value-impl.h"
#include "expression-test-support.h"

#include <iostream>

using std::shared_ptr;
using std::make_shared;

using testing::DefaultValue;
using testing::NiceMock;
using testing::Ref;
using testing::Return;
using testing::_;

class ExpressionRelationalTest : public ::testing::Test {
protected:
  shared_ptr<MockExpression> left  = make_shared<NiceMock<MockExpression>>();
  shared_ptr<MockExpression> right = make_shared<NiceMock<MockExpression>>();

  ExpressionTestSupport support;
  XPathEvaluationContext context = support.context();

  void SetUp() {
    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }
};

static bool noop(double left, double right) { return false; }

TEST_F(ExpressionRelationalTest, evaluates_both_arguments)
{
  ExpressionRelational expression(left, right, noop);

  EXPECT_CALL(*left, evaluate(Ref(context)));
  EXPECT_CALL(*right, evaluate(Ref(context)));

  expression.evaluate(context);
}

TEST_F(ExpressionRelationalTest, computes_less_than)
{
  auto expression = ExpressionRelational::LessThan(left, right);

  EXPECT_CALL(*left, evaluate(_)).WillRepeatedly(Return(0.1));
  EXPECT_CALL(*right, evaluate(_)).WillRepeatedly(Return(1.0));

  ASSERT_EQ(true, expression->evaluate(context).boolean());
}

TEST_F(ExpressionRelationalTest, computes_less_than_or_equal)
{
  auto expression = ExpressionRelational::LessThanOrEqual(left, right);

  EXPECT_CALL(*left, evaluate(_)).WillRepeatedly(Return(0.1));
  EXPECT_CALL(*right, evaluate(_)).WillRepeatedly(Return(0.1));

  ASSERT_EQ(true, expression->evaluate(context).boolean());
}

TEST_F(ExpressionRelationalTest, computes_greater_than)
{
  auto expression = ExpressionRelational::GreaterThan(left, right);

  EXPECT_CALL(*left, evaluate(_)).WillRepeatedly(Return(0.1));
  EXPECT_CALL(*right, evaluate(_)).WillRepeatedly(Return(0.1));

  ASSERT_EQ(false, expression->evaluate(context).boolean());
}

TEST_F(ExpressionRelationalTest, computes_greater_than_or_equal)
{
  auto expression = ExpressionRelational::GreaterThan(left, right);

  EXPECT_CALL(*left, evaluate(_)).WillRepeatedly(Return(1.0));
  EXPECT_CALL(*right, evaluate(_)).WillRepeatedly(Return(0.1));

  ASSERT_EQ(true, expression->evaluate(context).boolean());
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
