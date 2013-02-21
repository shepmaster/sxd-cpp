#include "function-last.h"

#include "document.h"

#include "gmock/gmock.h"
#include <iostream>

using std::shared_ptr;
using std::make_shared;

class FunctionLastTest : public ::testing::Test {
protected:
  Document doc;
  Nodeset nodes;
  XPathFunctionLibrary functions;
  shared_ptr<FunctionLast> function = make_shared<FunctionLast>();
};

TEST_F(FunctionLastTest, empty_context_returns_zero)
{
  auto context = make_shared<XPathEvaluationContext>(nodes, functions);

  auto result = function->evaluate(*context);

  ASSERT_EQ(XPathValue(0.0), result);
}

TEST_F(FunctionLastTest, returns_number_of_nodes_in_context)
{
  auto element = doc.new_element("element");
  nodes.add(element);
  auto context = make_shared<XPathEvaluationContext>(nodes, functions);

  auto result = function->evaluate(*context);

  ASSERT_EQ(XPathValue(1.0), result);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
