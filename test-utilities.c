#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "document.h"
#include "node.h"
#include "element.h"
#include "text-node.h"
#include "test-utilities.h"

node_t *
test_helper_new_node(document_t *doc, const char * const name)
{
  element_t *e;
  e = document_element_new(doc, name);
  return element_cast_to_node(e);
}

node_t *
test_helper_new_text_node(document_t *doc, const char * const text)
{
  text_node_t *tn;
  tn = document_text_node_new(doc, text);
  return text_node_cast_to_node(tn);
}

static void
test_output_fn(void *string_as_void, const char *format, ...)
{
  GString *string = string_as_void;
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
