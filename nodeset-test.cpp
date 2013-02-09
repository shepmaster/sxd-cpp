#include "nodeset.h"

#include "document.h"

#include "gmock/gmock.h"
#include <iostream>

class NodesetTest : public ::testing::Test {
protected:
  Document doc;
  Node *node;
  Nodeset nodeset;

  Node *new_node() {
    return doc.new_element("dummy-element");
  }

  void SetUp() {
    node = new_node();
  }

  void TearDown() {
    delete node;
  }
};

TEST_F(NodesetTest, add_node)
{
  nodeset.add(node);
  ASSERT_EQ(1, nodeset.count());
}

TEST_F(NodesetTest, get_node)
{
  const Node *n2;

  nodeset.add(node);
  n2 = nodeset[0];

  ASSERT_EQ(node, n2);
}

TEST_F(NodesetTest, add_nodeset)
{
  Node *n2;
  Nodeset ns2;

  n2 = new_node();

  nodeset.add(node);
  ns2.add(n2);

  nodeset.add_nodeset(ns2);
  ASSERT_EQ(2, nodeset.count());
  ASSERT_EQ(node, nodeset[0]);
  ASSERT_EQ(n2, nodeset[1]);

  delete n2;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
