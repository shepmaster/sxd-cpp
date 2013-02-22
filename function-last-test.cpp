#include "function-last.h"

#include "document.h"

#include "gmock/gmock.h"
#include <iostream>

class FunctionLastTest : public ::testing::Test {
protected:
  Document doc;
  Node *node = doc.new_element("context-node");
  Nodeset nodes;
  XPathFunctionLibrary functions;
  std::vector<XPathValue> arguments;
  FunctionLast function;
};

TEST_F(FunctionLastTest, empty_context_returns_zero)
{
  XPathEvaluationContext context(node, nodes, functions);

  auto result = function.evaluate(context, arguments);

  ASSERT_EQ(XPathValue(0.0), result);
}

TEST_F(FunctionLastTest, returns_number_of_nodes_in_context)
{
  nodes.add(doc.new_element("element"));
  XPathEvaluationContext context(node, nodes, functions);

  auto result = function.evaluate(context, arguments);

  ASSERT_EQ(XPathValue(1.0), result);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
