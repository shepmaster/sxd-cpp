#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "text-node.h"
#include "document.h"

TextNode::TextNode(Document *doc, const char *text) :
  Node(doc, NODE_TYPE_TEXT_NODE),
  text_(doc->intern(text))
{
}

TextNode::~TextNode()
{
}

const char *
TextNode::text()
{
  return text_;
}

void
TextNode::output(Output &output)
{
  output.output("%s", text_);
}

void
TextNode::change_document(Document *doc)
{
  Node::change_document(doc);
  text_ = doc->intern(text_);
}
