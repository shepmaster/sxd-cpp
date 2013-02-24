#include "expression-math.h"

#include "document.h"

#include "gmock/gmock.h"
#include "mock-xpath-expression.h"

#include <iostream>

using std::shared_ptr;
using std::make_shared;

using testing::DefaultValue;
using testing::NiceMock;
using testing::Ref;
using testing::Return;
using testing::_;

class ExpressionMathTest : public ::testing::Test {
protected:
  shared_ptr<MockExpression> left = make_shared<NiceMock<MockExpression>>();
  shared_ptr<MockExpression> right = make_shared<NiceMock<MockExpression>>();

  Document doc;
  Node *node = doc.new_element("context-node");
  Nodeset nodes;
  XPathFunctionLibrary functions;
  shared_ptr<XPathEvaluationContext> context;

  void SetUp() {
    context = make_shared<XPathEvaluationContext>(node, nodes, functions);
    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }
};

static double math_no_op(double left, double right) { return 0; }

TEST_F(ExpressionMathTest, evaluates_both_arguments)
{
  ExpressionMath expression(left, right, math_no_op);

  EXPECT_CALL(*left, evaluate(Ref(*context)));
  EXPECT_CALL(*right, evaluate(Ref(*context)));

  expression.evaluate(*context);
}

TEST_F(ExpressionMathTest, adds_arguments)
{
  auto expression = ExpressionMath::Addition(left, right);

  EXPECT_CALL(*left, evaluate(_)).WillRepeatedly(Return(-2.0));
  EXPECT_CALL(*right, evaluate(_)).WillRepeatedly(Return(3.0));

  auto result = expression->evaluate(*context);
  ASSERT_DOUBLE_EQ(1.0, result.number());
}

TEST_F(ExpressionMathTest, subtracts_arguments)
{
  auto expression = ExpressionMath::Subtraction(left, right);

  EXPECT_CALL(*left, evaluate(_)).WillRepeatedly(Return(-2.0));
  EXPECT_CALL(*right, evaluate(_)).WillRepeatedly(Return(3.0));

  auto result = expression->evaluate(*context);
  ASSERT_DOUBLE_EQ(-5.0, result.number());
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
