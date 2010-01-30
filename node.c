#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "node-internal.h"
#include "document-internal.h"

static void
node_free_children(gpointer node_as_gp, gpointer user_data_unused)
{
  node_t *node = node_as_gp;
  node_free(node);
}

void
node_destroy(node_t *node)
{
  if (! node) return;

  if (node->parent) {
    node_remove_child(node->parent, node);
  }

  g_list_foreach(node->children, node_free_children, NULL);

  document_stop_managing_node(node->doc, node);
  g_list_free(node->children);
}

void
node_free(node_t *node)
{
  node_destroy(node);
  free(node);
}

void
node_init(node_t * node, document_t *doc)
{
  node->doc = doc;
}

node_t *
node_cast_to_node(node_t *n)
{
  return n;
}

void
node_append_child(node_t *node, node_t *child)
{
  node->children = g_list_append(node->children, child);
  child->parent = node;
}

void
node_remove_child(node_t *node, node_t *child)
{
  node->children = g_list_remove(node->children, child);
}

node_t *
node_first_child(node_t *node)
{
  GList *item;

  item = g_list_first(node->children);
  if (item) return item->data;
  return NULL;
}

node_t *
node_parent(node_t *node)
{
  return node->parent;
}

node_t *
node_next_sibling(node_t *node)
{
  GList *self = g_list_find(node->parent->children, node);
  if (self) {
    GList *next = g_list_next(self);
    if (next) {
      return next->data;
    }
  }

  return NULL;
}

document_t *
node_document(node_t *node)
{
  return node->doc;
}

static void
node_change_document_children(gpointer node_as_gp, gpointer doc_as_gp)
{
  node_t *node = node_as_gp;
  document_t *doc = doc_as_gp;

  node_change_document(node, doc);
}

void
node_change_document(node_t *node, document_t *doc)
{
  document_stop_managing_node(node->doc, node);

  node->doc = doc;

  node->fn.change_document(node, doc);

  g_list_foreach(node->children, node_change_document_children, node->doc);
}
