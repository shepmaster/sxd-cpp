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
  node_t *parent;
  node_t *child;

  doc = document_new();
  parent = test_helper_new_node(doc, "parent");
  child = test_helper_new_node(doc, "child");

  node_append_child(parent, child);
  POINTERS_EQUAL(child, node_first_child(parent));
  POINTERS_EQUAL(parent, node_parent(child));

  node_free(parent);
  document_free(doc);
}

TEST(node, remove_child)
{
  document_t *doc;
  node_t *parent;
  node_t *child;

  doc = document_new();
  parent = test_helper_new_node(doc, "parent");
  child = test_helper_new_node(doc, "child");

  node_append_child(parent, child);
  node_remove_child(parent, child);
  POINTERS_EQUAL(NULL, node_first_child(parent));

  node_free(parent);
  node_free(child);
  document_free(doc);
}

TEST(node, free_child)
{
  document_t *doc;
  node_t *parent;
  node_t *child;

  doc = document_new();
  parent = test_helper_new_node(doc, "parent");
  child = test_helper_new_node(doc, "child");

  node_append_child(parent, child);
  node_free(child);
  POINTERS_EQUAL(NULL, node_first_child(parent));

  node_free(parent);
  document_free(doc);
}

TEST(node, sibling)
{
  document_t *doc;
  node_t *parent;
  node_t *child1;
  node_t *child2;

  doc = document_new();
  parent = test_helper_new_node(doc, "parent");
  child1 = test_helper_new_node(doc, "child1");
  child2 = test_helper_new_node(doc, "child2");

  node_append_child(parent, child1);
  node_append_child(parent, child2);

  POINTERS_EQUAL(child2, node_next_sibling(child1));

  node_free(parent);
  document_free(doc);
}

TEST(node, insert_next_sibling)
{
  document_t *doc = document_new();
  node_t *a = test_helper_new_node(doc, "a");
  node_t *b = test_helper_new_node(doc, "b");
  node_t *c = test_helper_new_node(doc, "c");

  node_insert_next_sibling(a, c);
  POINTERS_EQUAL(NULL, node_prev_sibling(a));
  POINTERS_EQUAL(c, node_next_sibling(a));
  POINTERS_EQUAL(a, node_prev_sibling(c));
  POINTERS_EQUAL(NULL, node_next_sibling(c));

  node_insert_next_sibling(a, b);
  POINTERS_EQUAL(NULL, node_prev_sibling(a));
  POINTERS_EQUAL(b, node_next_sibling(a));
  POINTERS_EQUAL(a, node_prev_sibling(b));
  POINTERS_EQUAL(c, node_next_sibling(b));
  POINTERS_EQUAL(b, node_prev_sibling(c));
  POINTERS_EQUAL(NULL, node_next_sibling(c));

  node_free(a);
  node_free(b);
  node_free(c);
  document_free(doc);
}

TEST(node, append_child_siblings)
{
  document_t *doc = document_new();
  node_t *parent = test_helper_new_node(doc, "parent");
  node_t *a = test_helper_new_node(doc, "a");
  node_t *b = test_helper_new_node(doc, "b");
  node_t *c = test_helper_new_node(doc, "c");

  node_append_child(parent, a);
  POINTERS_EQUAL(NULL, node_prev_sibling(a));
  POINTERS_EQUAL(NULL, node_next_sibling(a));

  node_append_child(parent, b);
  POINTERS_EQUAL(NULL, node_prev_sibling(a));
  POINTERS_EQUAL(b, node_next_sibling(a));
  POINTERS_EQUAL(a, node_prev_sibling(b));
  POINTERS_EQUAL(NULL, node_next_sibling(b));

  node_append_child(parent, c);
  POINTERS_EQUAL(NULL, node_prev_sibling(a));
  POINTERS_EQUAL(b, node_next_sibling(a));
  POINTERS_EQUAL(a, node_prev_sibling(b));
  POINTERS_EQUAL(c, node_next_sibling(b));
  POINTERS_EQUAL(b, node_prev_sibling(c));
  POINTERS_EQUAL(NULL, node_next_sibling(c));

  node_free(parent);
  document_free(doc);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
