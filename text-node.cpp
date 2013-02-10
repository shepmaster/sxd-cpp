#include <iostream>

#include "text-node.h"
#include "document.h"

TextNode::TextNode(Document *doc, const char *text) :
  Node(doc, NODE_TYPE_TEXT_NODE),
  _text(text)
{
}

const char *
TextNode::text()
{
  return _text.c_str();
}

void
TextNode::output(Output &output)
{
  output.output("%s", _text.c_str());
}
