#include "expression-function.h"

#include "document.h"

#include "gmock/gmock.h"
#include "mock-xpath-expression.h"
#include "mock-xpath-function.h"

#include <iostream>

using std::make_shared;

using testing::DefaultValue;
using testing::Ref;
using testing::Return;
using testing::_;

class ExpressionFunctionTest : public ::testing::Test {
protected:
  Document doc;
  Nodeset nodes;
  XPathFunctionLibrary functions;
  std::vector<std::shared_ptr<XPathExpression>> arguments;

  void SetUp() {
    auto function = make_shared<MockFunction>();
    EXPECT_CALL(*function, name()).WillRepeatedly(Return("foo"));
    EXPECT_CALL(*function, evaluate(_, _));//.WillRepeatedly(Return("foo"));
    functions.add(function);
    DefaultValue<XPathValue>::Set(XPathValue(0.0));
  }
};

TEST_F(ExpressionFunctionTest, evaluates_input_arguments)
{
  XPathEvaluationContext context(nodes, functions);

  auto argument = make_shared<MockExpression>();
  EXPECT_CALL(*argument, evaluate(Ref(context)));
  arguments.push_back(argument);

  ExpressionFunction expression("foo", arguments);

  expression.evaluate(context);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
