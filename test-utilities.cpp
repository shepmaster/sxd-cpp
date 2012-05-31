#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "document.h"
#include "node.h"
#include "element.h"
#include "text-node.h"
#include "test-utilities.h"

Node *
test_helper_new_node(document_t *doc, const char * const name)
{
  Element *e;
  e = document_element_new(doc, name);
  return e;
}

Node *
test_helper_new_text_node(document_t *doc, const char * const text)
{
  return document_text_node_new(doc, text);
}

StringOutput::StringOutput() :
  string_(g_string_new(NULL))
{}

StringOutput::~StringOutput()
{
  g_string_free(string_, TRUE);
}

void
StringOutput::output(const char *format, ...)
{
  va_list params;

  va_start(params, format);
  g_string_append_vprintf(string_, format, params);
  va_end(params);
}

const char *
StringOutput::string()
{
  return string_->str;
}


nodeset_t *
nodeset_new_with_nodes(Node *first, ...)
{
  nodeset_t *ns;
  Node *node;
  va_list ap;

  ns = nodeset_new();
  nodeset_add(ns, first);

  va_start(ap, first);
  while ((node = va_arg(ap, Node *))) {
    nodeset_add(ns, node);
  }
  va_end(ap);

  return ns;
}
