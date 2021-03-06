#include "node.h"

#include "document.h"

#include "gmock/gmock.h"
#include <iostream>

class NodeTest : public ::testing::Test {
protected:
  Document doc;

  Node *new_node() {
    return doc.new_element("dummy-element");
  }
};

TEST_F(NodeTest, append_child)
{
  Node *parent;
  Node *child;

  parent = new_node();
  child = new_node();

  parent->append_child(child);
  ASSERT_EQ(child, parent->first_child());
  ASSERT_EQ(parent, child->parent());
}

TEST_F(NodeTest, remove_child)
{
  Node *parent;
  Node *child;

  parent = new_node();
  child = new_node();

  parent->append_child(child);
  parent->remove_child(child);
  ASSERT_EQ(NULL, parent->first_child());
}

TEST_F(NodeTest, sibling)
{
  Node *parent;
  Node *child1;
  Node *child2;

  parent = new_node();
  child1 = new_node();
  child2 = new_node();

  parent->append_child(child1);
  parent->append_child(child2);

  ASSERT_EQ(child2, child1->next_sibling());
}

TEST_F(NodeTest, append_child_siblings)
{
  Node *parent = new_node();
  Node *a = new_node();
  Node *b = new_node();
  Node *c = new_node();

  parent->append_child(a);
  ASSERT_EQ(NULL, a->prev_sibling());
  ASSERT_EQ(NULL, a->next_sibling());

  parent->append_child(b);
  ASSERT_EQ(NULL, a->prev_sibling());
  ASSERT_EQ(b, a->next_sibling());
  ASSERT_EQ(a, b->prev_sibling());
  ASSERT_EQ(NULL, b->next_sibling());

  parent->append_child(c);
  ASSERT_EQ(NULL, a->prev_sibling());
  ASSERT_EQ(b, a->next_sibling());
  ASSERT_EQ(a, b->prev_sibling());
  ASSERT_EQ(c, b->next_sibling());
  ASSERT_EQ(b, c->prev_sibling());
  ASSERT_EQ(NULL, c->next_sibling());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
