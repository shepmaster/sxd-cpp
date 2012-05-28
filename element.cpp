#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "document-internal.h"
#include "element-internal.h"
#include "node-internal.h"

void
element_free(element_t *e)
{
  delete e;
}

static GHashTable *
element_attributes_new(void)
{
  return g_hash_table_new(g_str_hash, g_str_equal);
}

element_t *
element_new(document_t *doc, const char * const name)
{
  return new Element(doc, name);
}

node_t *
element_cast_to_node(element_t *e)
{
  return e;
}

const char *
element_name(element_t *e)
{
  return e->name();
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
  element->set_attribute(name, value);
}

const char *
element_get_attribute(element_t *element, const char * const name)
{
  return element->get_attribute(name);
}

typedef struct {
  document_t *doc;
  GHashTable *new_attributes;
} change_attributes_t;

static void
element_change_document_attributes(gpointer name_as_gp, gpointer value_as_gp, gpointer ca_as_gp)
{
  const char *name = (const char *)name_as_gp;
  const char *value = (const char *)value_as_gp;
  change_attributes_t *ca = (change_attributes_t *)ca_as_gp;

  element_set_attribute1(ca->doc, ca->new_attributes, name, value);
}

void
element_change_document(node_t *node, document_t *doc)
{
  element_t *element;
  element = (element_t *)node;
  element->change_document(doc);
}

static void
element_output_attribute(gpointer name_as_gp, gpointer value_as_gp, gpointer output_as_gp)
{
  const char * const name = (const char * const)name_as_gp;
  const char * const value = (const char * const)value_as_gp;
  output_t *output = (output_t *)output_as_gp;

  output->fn(output->data, " %s=\"%s\"", name, value);
}

#include <stdarg.h>

static void
fprintf_wrapper(void *data, const char *format, ...)
{
  FILE *stream = (FILE *)data;
  va_list params;

  va_start(params, format);
  vfprintf(stream, format, params);
  va_end(params);
}

Element::Element(document_t *doc, const char * const name) :
  Node(doc, NODE_TYPE_ELEMENT),
  name_(document_intern(doc, name)),
  attributes(element_attributes_new())
{
}

Element::~Element()
{
  g_hash_table_destroy(this->attributes);
}

void
Element::output(output_t *output)
{
  output_t def_output;

  if (output == NULL) {
    def_output.fn = fprintf_wrapper;
    def_output.data = stderr;
    output = &def_output;
  }

  output->fn(output->data, "<%s", name_);
  g_hash_table_foreach(this->attributes, element_output_attribute, output);
  if (first_child()) {
    output->fn(output->data, ">");
    output_children(output);
    output->fn(output->data, "</%s>", name_);
  } else {
    output->fn(output->data, " />");
  }
}

void
Element::set_attribute(const char * const name, const char * const value)
{
  element_set_attribute1(doc, attributes, name, value);
}

const char *
Element::get_attribute(const char * const name)
{
  return (char *)g_hash_table_lookup(attributes, name);
}

void
Element::change_document(document_t *doc)
{
  change_attributes_t ca;

  Node::change_document(doc);

  this->name_ = document_intern(doc, name_);

  ca.new_attributes = element_attributes_new();
  ca.doc = doc;
  g_hash_table_foreach(attributes, element_change_document_attributes, &ca);
  g_hash_table_destroy(attributes);
  this->attributes = ca.new_attributes;
}

const char *
Element::name()
{
  return this->name_;
}
