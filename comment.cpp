#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node-internal.h"
#include "comment.h"

const char *
comment_text(comment_t *c)
{
  return c->text();
}

node_t *
comment_cast_to_node(comment_t *c)
{
  return c;
}

void
comment_free(comment_t *c)
{
  delete c;
}

void
comment_free_node(node_t *node)
{
  comment_free((comment_t *)node);
}

comment_t *
comment_new(document_t *doc, const char * const text)
{
  return new Comment(doc, text);
}

Comment::Comment(document_t *doc, const char * const text) :
  Node(doc, NODE_TYPE_COMMENT),
  text_(strdup(text))
{
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

void
Comment::output(output_t *output)
{
}
