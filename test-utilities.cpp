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

static void
test_output_fn(void *string_as_void, const char *format, ...)
{
  GString *string = (GString *)string_as_void;
  va_list params;

  va_start(params, format);
  g_string_append_vprintf(string, format, params);
  va_end(params);
}

void
test_output_init(test_output_t *to)
{
  to->string = g_string_new(NULL);
  to->out.fn = test_output_fn;
  to->out.data = to->string;
}

void
test_output_destroy(test_output_t *to)
{
  g_string_free(to->string, TRUE);
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
