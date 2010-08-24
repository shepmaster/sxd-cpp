#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "node-internal.h"
#include "document-internal.h"

void
node_foreach_child(node_t *node, node_foreach_child_t fn, gpointer user_data)
{
  node_t *child;
  for (child = node->first_child; child; child = child->next_sibling) {
    fn(child, user_data);
  }
}

static void
node_free_children(node_t *node, gpointer user_data_unused)
{
  node_free(node);
}

void
node_destroy(node_t *node)
{
  if (! node) return;

  if (node->parent) {
    node_remove_child(node->parent, node);
  }

  node_foreach_child(node, node_free_children, NULL);

  document_stop_managing_node(node->doc, node);
}

void
node_free(node_t *node)
{
  node->fn.free_node(node);
}

void
node_init(node_t * node, document_t *doc)
{
  node->doc = doc;
}

node_type_t
node_type(node_t *node)
{
  return node->type;
}

node_t *
node_cast_to_node(node_t *n)
{
  return n;
}

void
node_append_child(node_t *node, node_t *child)
{
  node_t *my_child;

  my_child = node->first_child;
  if (my_child) {
    while (my_child->next_sibling) {
      my_child = my_child->next_sibling;
    }
    my_child->next_sibling = child;
  } else {
    node->first_child = child;
  }
  child->parent = node;
}

void
node_remove_child(node_t *node, node_t *child)
{
  if (child->prev_sibling) {
    child->prev_sibling->next_sibling = child->next_sibling;
  }

  if (child->next_sibling) {
    child->next_sibling->prev_sibling = child->prev_sibling;
  }

  if (child->parent && child->parent->first_child == child) {
    child->parent->first_child = child->next_sibling;
  }
  child->parent = NULL;
}

node_t *
node_first_child(node_t *node)
{
  return node->first_child;
}

node_t *
node_parent(node_t *node)
{
  return node->parent;
}

node_t *
node_next_sibling(node_t *node)
{
  return node->next_sibling;
}

void
node_insert_next_sibling(node_t *node, node_t *sibling)
{
  sibling->next_sibling = node->next_sibling;
  node->next_sibling = sibling;
}

document_t *
node_document(node_t *node)
{
  return node->doc;
}

static void
node_change_document_children(node_t *node, gpointer doc_as_gp)
{
  document_t *doc = doc_as_gp;

  node_change_document(node, doc);
}

void
node_change_document(node_t *node, document_t *doc)
{
  document_stop_managing_node(node->doc, node);

  node->doc = doc;

  node->fn.change_document(node, doc);

  node_foreach_child(node, node_change_document_children, node->doc);
}

void
node_output(node_t *node, output_t *output)
{
  switch(node->type) {
  case NODE_TYPE_ELEMENT:
    element_output((element_t *)node, output);
    break;
  case NODE_TYPE_TEXT_NODE:
    text_node_output((text_node_t *)node, output);
    break;
 }
}

static void
node_output_wrapper(node_t *node, gpointer output_as_gp)
{
  node_output(node, output_as_gp);
}

void
node_output_children(node_t *node, output_t *output)
{
  node_foreach_child(node, node_output_wrapper, output);
}
