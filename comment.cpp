#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comment.h"

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
