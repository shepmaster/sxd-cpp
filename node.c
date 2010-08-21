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
  node->fn.free_node(node);
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
  child->parent = NULL;
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

typedef struct {
  nodeset_t *nodeset;
  xpath_predicate_type_t select;
} select_xpath_children_t;

static void
node_select_xpath_children(gpointer node_as_gp, gpointer data_as_gp)
{
  node_t *node = node_as_gp;
  select_xpath_children_t *data = data_as_gp;
  int should_add = FALSE;

  switch (node->type) {
  case NODE_TYPE_ELEMENT:
    should_add = data->select & XPATH_PREDICATE_ELEMENT;
    break;
  case NODE_TYPE_TEXT_NODE:
    should_add = data->select & XPATH_PREDICATE_TEXT_NODE;
    break;
  }

  if (should_add) {
    nodeset_add(data->nodeset, node);
  }
}

nodeset_t *
node_select_xpath(node_t *node, xpath_predicate_type_t select)
{
  select_xpath_children_t data;

  data.nodeset = nodeset_new();
  data.select = select;

  g_list_foreach(node->children, node_select_xpath_children, &data);

  return data.nodeset;
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
node_output_wrapper(gpointer node_as_gp, gpointer output_as_gp)
{
  node_output(node_as_gp, output_as_gp);
}

void
node_output_children(node_t *node, output_t *output)
{
  g_list_foreach(node->children, node_output_wrapper, output);
}
