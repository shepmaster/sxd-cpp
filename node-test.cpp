#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "node.h"
#include "test-utilities.h"

TEST_GROUP(node)
{};

TEST(node, append_child)
{
  document_t *doc;
  Node *parent;
  Node *child;

  doc = document_new();
  parent = test_helper_new_node(doc, "parent");
  child = test_helper_new_node(doc, "child");

  parent->append_child(child);
  POINTERS_EQUAL(child, parent->first_child());
  POINTERS_EQUAL(parent, child->parent());

  delete parent;
  document_free(doc);
}

TEST(node, remove_child)
{
  document_t *doc;
  Node *parent;
  Node *child;

  doc = document_new();
  parent = test_helper_new_node(doc, "parent");
  child = test_helper_new_node(doc, "child");

  parent->append_child(child);
  parent->remove_child(child);
  POINTERS_EQUAL(NULL, parent->first_child());

  delete parent;
  delete child;
  document_free(doc);
}

TEST(node, free_child)
{
  document_t *doc;
  Node *parent;
  Node *child;

  doc = document_new();
  parent = test_helper_new_node(doc, "parent");
  child = test_helper_new_node(doc, "child");

  parent->append_child(child);
  delete child;
  POINTERS_EQUAL(NULL, parent->first_child());

  delete parent;
  document_free(doc);
}

TEST(node, sibling)
{
  document_t *doc;
  Node *parent;
  Node *child1;
  Node *child2;

  doc = document_new();
  parent = test_helper_new_node(doc, "parent");
  child1 = test_helper_new_node(doc, "child1");
  child2 = test_helper_new_node(doc, "child2");

  parent->append_child(child1);
  parent->append_child(child2);

  POINTERS_EQUAL(child2, child1->next_sibling());

  delete parent;
  document_free(doc);
}

TEST(node, insert_next_sibling)
{
  document_t *doc = document_new();
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
  document_free(doc);
}

TEST(node, append_child_siblings)
{
  document_t *doc = document_new();
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
  document_free(doc);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
