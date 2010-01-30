#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "document.h"
#include "node-internal.h"
#include "element-internal.h"
#include "intern.h"

struct documentS {
  intern_t *dict;
  unsigned int managed_node_count;
};

void
document_free(document_t *doc)
{
  if (! doc) return;
  
  intern_free(doc->dict);
  free(doc);
}

document_t *
document_new(void)
{
  document_t *doc;

  doc = calloc(1, sizeof(*doc));
  doc->dict = intern_new();

  return doc;
}

node_t *
document_node_new(document_t *doc, const char * const name)
{
  node_t *n = node_new(doc, name);
  if (n) {
    doc->managed_node_count++;
  }
  return n;
}

element_t *
document_element_new(document_t *doc, const char * const name)
{
  element_t *e = element_new(doc, name);
  if (e) {
    doc->managed_node_count++;
  }
  return e;
}

text_node_t *
document_text_node_new(document_t *doc, const char * const text)
{
  return text_node_new(doc, text);
}

const char *
document_intern(document_t *doc, const char * const string)
{
  return intern_intern(doc->dict, string);
}

unsigned int
document_managed_node_count(document_t *doc)
{
  return doc->managed_node_count;
}

void
document_stop_managing_node(document_t *doc, node_t *node)
{
  doc->managed_node_count--;
}

void
document_manage_node(document_t *doc, node_t *node)
{
  node_change_document(node, doc);
  doc->managed_node_count++;
}
