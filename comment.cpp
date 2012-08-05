#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comment.h"

Comment::Comment(Document *doc, const char * const text) :
  Node(doc, NODE_TYPE_COMMENT),
  text_(text)
{
}

Comment::~Comment()
{
}

const char *
Comment::text()
{
  return text_.c_str();
}

void
Comment::output(Output &output)
{
}
