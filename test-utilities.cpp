#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "document.h"
#include "node.h"
#include "element.h"
#include "text-node.h"
#include "test-utilities.h"

Node *
test_helper_new_node(Document &doc, const char * const name)
{
  return doc.new_element(name);
}

Node *
test_helper_new_text_node(Document *doc, const char * const text)
{
  return doc->new_text_node(text);
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


Nodeset *
nodeset_new_with_nodes(Node *first, ...)
{
  Nodeset *ns;
  Node *node;
  va_list ap;

  ns = new Nodeset();
  ns->add(first);

  va_start(ap, first);
  while ((node = va_arg(ap, Node *))) {
    ns->add(node);
  }
  va_end(ap);

  return ns;
}
