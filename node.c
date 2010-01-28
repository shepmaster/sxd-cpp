#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "node-internal.h"
#include "document-internal.h"

struct nodeS {
  document_t *doc;
  const char *name;
  GHashTable *attributes;
  struct nodeS *parent;
  GList *children;
};

static void
node_free_children(gpointer node_as_gp, gpointer user_data_unused)
{
  node_t *node = node_as_gp;
  node_free(node);
}

void
node_free(node_t *node)
{
  if (! node) return;

  if (node->parent) {
    node_remove_child(node->parent, node);
  }

  g_list_foreach(node->children, node_free_children, NULL);

  document_stop_managing_node(node->doc, node);
  g_hash_table_destroy(node->attributes);
  g_list_free(node->children);

  free(node);
}

static GHashTable *
node_attributes_new(void)
{
  return g_hash_table_new(g_str_hash, g_str_equal);
}

node_t *
node_new(document_t *doc, const char * const name)
{
  node_t *node;

  node = calloc(1, sizeof(*node));
  node->doc = doc;
  node->name = document_intern(node->doc, name);
  node->attributes = node_attributes_new();

  return node;
}

const char *
node_name(node_t *node)
{
  return node->name;
}

static void
node_set_attribute1(document_t *doc, GHashTable *attributes, const char * const name, const char * const value)
{
  const char *n = document_intern(doc, name);
  const char *v = document_intern(doc, value);

  g_hash_table_insert(attributes, (gpointer)n, (gpointer)v);
}

void
node_set_attribute(node_t *node, const char * const name, const char * const value)
{
  node_set_attribute1(node->doc, node->attributes, name, value);
}

const char *
node_get_attribute(node_t *node, const char * const name)
{
  return g_hash_table_lookup(node->attributes, name);
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

typedef struct {
  node_t *node;
  GHashTable *new_attributes;
} change_attributes_t;

static void
node_change_document_attributes(gpointer name_as_gp, gpointer value_as_gp, gpointer ca_as_gp)
{
  const char *name = name_as_gp;
  const char *value = value_as_gp;
  change_attributes_t *ca = ca_as_gp;

  node_set_attribute1(ca->node->doc, ca->new_attributes, name, value);
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
  change_attributes_t ca;

  document_stop_managing_node(node->doc, node);

  node->doc = doc;
  node->name = document_intern(doc, node->name);

  ca.new_attributes = node_attributes_new();
  ca.node = node;
  g_hash_table_foreach(node->attributes, node_change_document_attributes, &ca);
  g_hash_table_destroy(node->attributes);
  node->attributes = ca.new_attributes;

  g_list_foreach(node->children, node_change_document_children, node->doc);
}
