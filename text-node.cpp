#include <iostream>

#include "text-node.h"
#include "document.h"

TextNode::TextNode(Document *doc, const char *text) :
  Node(doc, NODE_TYPE_TEXT_NODE),
  _text(text)
{
}

const std::string
TextNode::text() const
{
  return _text;
}

void
TextNode::output(Output &output) const
{
  output.output("%s", _text.c_str());
}

std::ostream&
TextNode::to_stream(std::ostream& os) const
{
  os << "Text '";
  if (_text.length() > 16) {
    os << _text.substr(0, 16).append("...");
  } else {
    os << _text;
  }
  return os << "' <" << this << ">";
}
