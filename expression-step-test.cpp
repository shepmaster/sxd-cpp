#include "expression-step.h"

#include "document.h"

#include "gmock/gmock.h"
#include "mock-xpath-axis.h"
#include "mock-xpath-node-test.h"
#include "mock-xpath-evaluation-context.h"

#include <iostream>

using std::shared_ptr;
using std::make_shared;

using testing::DefaultValue;
using testing::NiceMock;
using testing::Return;
using testing::_;

class ExpressionStepTest : public ::testing::Test {
protected:
  Document doc;
  Element *top_element = doc.new_element("top");

  shared_ptr<MockAxis> axis = make_shared<MockAxis>();
  shared_ptr<MockNodeTest> node_test = make_shared<MockNodeTest>();

  NiceMock<MockEvaluationContext> context;
  shared_ptr<MockEvaluationContext> sub_context =
    make_shared<NiceMock<MockEvaluationContext>>();

  void SetUp() {
    EXPECT_CALL(context, node()).WillRepeatedly(Return(top_element));
    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }
};

TEST_F(ExpressionStepTest, axis_is_passed_the_node)
{
  EXPECT_CALL(*axis, select_nodes(top_element, _, _));

  ExpressionStep step(axis, node_test);

  step.evaluate(context);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
