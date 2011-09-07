#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node-internal.h"
#include "comment.h"

struct commentS {
  node_t node;
  char *text;
};

const char *
comment_text(comment_t *c)
{
  return c->text;
}

node_t *
comment_cast_to_node(comment_t *c)
{
  return &c->node;
}

void
comment_free(comment_t *c)
{
  free(c->text);
  free(c);
}

comment_t *
comment_new(document_t *doc, const char * const text)
{
  comment_t *c;

  c = calloc(sizeof(*c), 1);

  node_init(&c->node, doc);
  c->node.type = NODE_TYPE_COMMENT;

  c->text = strdup(text);
  return c;
}
