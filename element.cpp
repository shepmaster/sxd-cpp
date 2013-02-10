#include <iostream>
#include <glib.h>

#include "document.h"

static GHashTable *
element_attributes_new(void)
{
  return g_hash_table_new(g_str_hash, g_str_equal);
}

static void
element_set_attribute1(Document *doc, GHashTable *attributes, const char * const name, const char * const value)
{
  const char *n = doc->intern(name);
  const char *v = doc->intern(value);

  g_hash_table_insert(attributes, (gpointer)n, (gpointer)v);
}

typedef struct {
  Document *doc;
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

static void
element_output_attribute(gpointer name_as_gp, gpointer value_as_gp, gpointer output_as_gp)
{
  const char * const name = (const char * const)name_as_gp;
  const char * const value = (const char * const)value_as_gp;
  Output *output = (Output *)output_as_gp;

  output->output(" %s=\"%s\"", name, value);
}

Element::Element(Document *doc, const char * const name) :
  Node(doc, NODE_TYPE_ELEMENT),
  name_(doc->intern(name)),
  attributes(element_attributes_new())
{
}

Element::~Element()
{
  g_hash_table_destroy(this->attributes);
}

void
Element::output(Output &output)
{
  output.output("<%s", name_);
  g_hash_table_foreach(this->attributes, element_output_attribute, &output);
  if (first_child()) {
    output.output(">");
    output_children(output);
    output.output("</%s>", name_);
  } else {
    output.output(" />");
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
Element::change_document(Document *doc)
{
  Node::change_document(doc);

  this->name_ = doc->intern(name_);

  change_attributes_t ca;
  ca.new_attributes = element_attributes_new();
  ca.doc = doc;
  g_hash_table_foreach(attributes, element_change_document_attributes, &ca);
  g_hash_table_destroy(attributes);
  this->attributes = ca.new_attributes;
}

const char *
Element::name() const
{
  return this->name_;
}

std::ostream &
Element::to_stream(std::ostream& os) const
{
  return os << "Element '" << this->name() << "' <" << this << ">";
}
