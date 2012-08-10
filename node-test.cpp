#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "node.h"
#include "test-utilities.h"

TEST_GROUP(node)
{};

TEST(node, append_child)
{
  Document doc;
  Node *parent;
  Node *child;

  parent = test_helper_new_node(doc, "parent");
  child = test_helper_new_node(doc, "child");

  parent->append_child(child);
  POINTERS_EQUAL(child, parent->first_child());
  POINTERS_EQUAL(parent, child->parent());

  delete parent;
}

TEST(node, remove_child)
{
  Document doc;
  Node *parent;
  Node *child;

  parent = test_helper_new_node(doc, "parent");
  child = test_helper_new_node(doc, "child");

  parent->append_child(child);
  parent->remove_child(child);
  POINTERS_EQUAL(NULL, parent->first_child());

  delete parent;
  delete child;
}

TEST(node, free_child)
{
  Document doc;
  Node *parent;
  Node *child;

  parent = test_helper_new_node(doc, "parent");
  child = test_helper_new_node(doc, "child");

  parent->append_child(child);
  delete child;
  POINTERS_EQUAL(NULL, parent->first_child());

  delete parent;
}

TEST(node, sibling)
{
  Document doc;
  Node *parent;
  Node *child1;
  Node *child2;

  parent = test_helper_new_node(doc, "parent");
  child1 = test_helper_new_node(doc, "child1");
  child2 = test_helper_new_node(doc, "child2");

  parent->append_child(child1);
  parent->append_child(child2);

  POINTERS_EQUAL(child2, child1->next_sibling());

  delete parent;
}

TEST(node, insert_next_sibling)
{
  Document doc;
  Node *a = test_helper_new_node(doc, "a");
  Node *b = test_helper_new_node(doc, "b");
  Node *c = test_helper_new_node(doc, "c");

  a->insert_next_sibling(c);
  POINTERS_EQUAL(NULL, a->prev_sibling());
  POINTERS_EQUAL(c, a->next_sibling());
  POINTERS_EQUAL(a, c->prev_sibling());
  POINTERS_EQUAL(NULL, c->next_sibling());

  a->insert_next_sibling(b);
  POINTERS_EQUAL(NULL, a->prev_sibling());
  POINTERS_EQUAL(b, a->next_sibling());
  POINTERS_EQUAL(a, b->prev_sibling());
  POINTERS_EQUAL(c, b->next_sibling());
  POINTERS_EQUAL(b, c->prev_sibling());
  POINTERS_EQUAL(NULL, c->next_sibling());

  delete a;
  delete b;
  delete c;
}

TEST(node, append_child_siblings)
{
  Document doc;
  Node *parent = test_helper_new_node(doc, "parent");
  Node *a = test_helper_new_node(doc, "a");
  Node *b = test_helper_new_node(doc, "b");
  Node *c = test_helper_new_node(doc, "c");

  parent->append_child(a);
  POINTERS_EQUAL(NULL, a->prev_sibling());
  POINTERS_EQUAL(NULL, a->next_sibling());

  parent->append_child(b);
  POINTERS_EQUAL(NULL, a->prev_sibling());
  POINTERS_EQUAL(b, a->next_sibling());
  POINTERS_EQUAL(a, b->prev_sibling());
  POINTERS_EQUAL(NULL, b->next_sibling());

  parent->append_child(c);
  POINTERS_EQUAL(NULL, a->prev_sibling());
  POINTERS_EQUAL(b, a->next_sibling());
  POINTERS_EQUAL(a, b->prev_sibling());
  POINTERS_EQUAL(c, b->next_sibling());
  POINTERS_EQUAL(b, c->prev_sibling());
  POINTERS_EQUAL(NULL, c->next_sibling());

  delete parent;
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
