#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node-internal.h"
#include "comment.h"

struct commentS {
  node_t node;
  Comment *c;
};

const char *
comment_text(comment_t *c)
{
  return c->c->text();
}

node_t *
comment_cast_to_node(comment_t *c)
{
  return &c->node;
}

void
comment_free(comment_t *c)
{
  delete c->c;
  free(c);
}

void
comment_free_node(node_t *node)
{
  comment_free((comment_t *)node);
}

comment_t *
comment_new(document_t *doc, const char * const text)
{
  comment_t *c;

  c = (comment_t *)calloc(sizeof(*c), 1);
  c->c = new Comment(&c->node, doc, text);
  return c;
}

Comment::Comment(node_t *node, document_t *doc, const char * const text) :
  node(node), text_(strdup(text))
{
  node_init(node, doc);
  node->type = NODE_TYPE_COMMENT;
  node->fn.free_node = comment_free_node;
}

Comment::~Comment()
{
  free(text_);
}

const char *
Comment::text()
{
  return text_;
}
