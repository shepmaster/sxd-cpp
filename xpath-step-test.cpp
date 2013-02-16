#include "xpath-step.h"

#include "document.h"

#include "gmock/gmock.h"
#include <iostream>

using std::shared_ptr;
using std::make_shared;
using testing::_;

class MockAxis : public XPathAxis {
public:
  MOCK_METHOD3(select_nodes, void(Node *node, XPathNodeTest const &test, Nodeset &result));
};

class MockNodeTest : public XPathNodeTest {
public:
  MOCK_CONST_METHOD2(test, void(Node *node, Nodeset &result));
};

class XPathStepTest : public ::testing::Test {
protected:
  Document doc;
  shared_ptr<MockAxis> axis = make_shared<MockAxis>();
  shared_ptr<MockNodeTest> node_test = make_shared<MockNodeTest>();
  Nodeset nodes;
};

TEST_F(XPathStepTest, axis_is_passed_the_node)
{
  auto top_element = doc.new_element("top");

  XPathStep step(axis, node_test);

  EXPECT_CALL(*axis, select_nodes(top_element, _, nodes));
  step.select_nodes(top_element, nodes);

  delete top_element;
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
