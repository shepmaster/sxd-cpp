#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "text-node.h"

struct text_nodeS {
  document_t *doc;
  char *text;
};

void
text_node_free(text_node_t *tn)
{
  if (! tn) return;

  free(tn->text);
  free(tn);
}

text_node_t *
text_node_new(document_t *doc, const char * const text)
{
  text_node_t *tn;

  tn = calloc(1, sizeof(*tn));
  tn->doc = doc;
  tn->text = strdup(text);
  return tn;
}

const char *
text_node_text(text_node_t *tn)
{
  return tn->text;
}
