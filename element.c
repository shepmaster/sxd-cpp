#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "document-internal.h"
#include "element-internal.h"
#include "node-internal.h"

struct elementS {
  node_t node;
  const char *name;
  GHashTable *attributes;
};

void
element_free(element_t *e)
{
  if (! e) return;

  node_destroy(&e->node);
  g_hash_table_destroy(e->attributes);

  free(e);
}

static void
element_free_node(node_t *node)
{
  element_free((element_t *)node);
}

static GHashTable *
element_attributes_new(void)
{
  return g_hash_table_new(g_str_hash, g_str_equal);
}

element_t *
element_new(document_t *doc, const char * const name)
{
  element_t *e;

  e = calloc(1, sizeof(*e));
  node_init(&e->node, doc);
  e->node.fn.free_node = element_free_node;
  e->node.fn.change_document = element_change_document;
  e->name = document_intern(e->node.doc, name);
  e->attributes = element_attributes_new();

  return e;
}

node_t *
element_cast_to_node(element_t *e)
{
  return (node_t *)e;
}

const char *
element_name(element_t *e)
{
  return e->name;
}

static void
element_set_attribute1(document_t *doc, GHashTable *attributes, const char * const name, const char * const value)
{
  const char *n = document_intern(doc, name);
  const char *v = document_intern(doc, value);

  g_hash_table_insert(attributes, (gpointer)n, (gpointer)v);
}

void
element_set_attribute(element_t *element, const char * const name, const char * const value)
{
  element_set_attribute1(element->node.doc, element->attributes, name, value);
}

const char *
element_get_attribute(element_t *element, const char * const name)
{
  return g_hash_table_lookup(element->attributes, name);
}
 
typedef struct {
  element_t *element;
  GHashTable *new_attributes;
} change_attributes_t;

static void
element_change_document_attributes(gpointer name_as_gp, gpointer value_as_gp, gpointer ca_as_gp)
{
  const char *name = name_as_gp;
  const char *value = value_as_gp;
  change_attributes_t *ca = ca_as_gp;

  element_set_attribute1(ca->element->node.doc, ca->new_attributes, name, value);
}

void
element_change_document(node_t *node, document_t *doc)
{
  element_t *element;
  change_attributes_t ca;

  element = (element_t *)node;

  element->name = document_intern(doc, element->name);

  ca.new_attributes = element_attributes_new();
  ca.element = element;
  g_hash_table_foreach(element->attributes, element_change_document_attributes, &ca);
  g_hash_table_destroy(element->attributes);
  element->attributes = ca.new_attributes;
}
