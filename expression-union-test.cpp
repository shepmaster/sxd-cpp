#include "expression-union.h"

#include "gmock/gmock.h"
#include "mock-xpath-expression.h"
#include "mock-xpath-evaluation-context.h"

#include <iostream>

using std::shared_ptr;
using std::make_shared;

using testing::DefaultValue;
using testing::NiceMock;
using testing::Ref;
using testing::Return;
using testing::ValuesIn;
using testing::_;

class ExpressionUnionTest : public ::testing::Test {
protected:
  shared_ptr<MockExpression> left = make_shared<NiceMock<MockExpression>>();
  shared_ptr<MockExpression> right = make_shared<NiceMock<MockExpression>>();

  NiceMock<MockEvaluationContext> context;

  void SetUp() {
    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }
};

TEST_F(ExpressionUnionTest, evaluates_both_arguments)
{
  ExpressionUnion expression(left, right);

  EXPECT_CALL(*left, evaluate(Ref(context)));
  EXPECT_CALL(*right, evaluate(Ref(context)));

  expression.evaluate(context);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
