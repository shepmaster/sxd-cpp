#include <iostream>

#include "comment.h"

Comment::Comment(Document *doc, const std::string text) :
  Node(doc, NODE_TYPE_COMMENT),
  _text(text)
{
}

const std::string
Comment::text() const
{
  return _text;
}

void
Comment::output(Output &output) const
{
}
