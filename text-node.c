#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "text-node.h"
#include "node-internal.h"
#include "document-internal.h"

struct text_nodeS {
  node_t node;
  const char *text;
};

void
text_node_free(text_node_t *tn)
{
  if (! tn) return;

  node_destroy(&tn->node);

  free(tn);
}

static void
text_node_free_node(node_t *node)
{
  text_node_free((text_node_t *)node);
}

static void
text_node_change_document(node_t *node, document_t *doc)
{
  text_node_t *tn = (text_node_t *)node;
  tn->text = document_intern(doc, tn->text);
}

text_node_t *
text_node_new(document_t *doc, const char * const text)
{
  text_node_t *tn;

  tn = calloc(1, sizeof(*tn));

  node_init(&tn->node, doc);
  tn->node.fn.free_node = text_node_free_node;
  tn->node.fn.change_document = text_node_change_document;

  tn->text = document_intern(doc, text);
  return tn;
}

const char *
text_node_text(text_node_t *tn)
{
  return tn->text;
}
