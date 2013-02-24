#include "expression-addition.h"

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

class ExpressionAdditionTest : public ::testing::Test {
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

TEST_F(ExpressionAdditionTest, evaluates_both_arguments)
{
  ExpressionAddition expression(left, right);

  EXPECT_CALL(*left, evaluate(Ref(*context)));
  EXPECT_CALL(*right, evaluate(Ref(*context)));

  expression.evaluate(*context);
}

TEST_F(ExpressionAdditionTest, adds_arguments_together)
{
  ExpressionAddition expression(left, right);

  EXPECT_CALL(*left, evaluate(_)).WillRepeatedly(Return(-2.0));
  EXPECT_CALL(*right, evaluate(_)).WillRepeatedly(Return(3.0));

  auto result = expression.evaluate(*context);
  ASSERT_DOUBLE_EQ(1.0, result.number());
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
