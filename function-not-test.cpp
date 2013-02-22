#include "function-not.h"

#include "document.h"

#include "gmock/gmock.h"
#include <iostream>

class FunctionNotTest : public ::testing::Test {
protected:
  Document doc;
  Nodeset nodes;
  XPathFunctionLibrary functions;
  std::vector<XPathValue> arguments;
  FunctionNot function;
};

TEST_F(FunctionNotTest, inverts_input_boolean)
{
  XPathEvaluationContext context(nodes, functions);

  arguments.push_back(XPathValue(false));

  auto result = function.evaluate(context, arguments);

  ASSERT_EQ(XPathValue(true), result);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
