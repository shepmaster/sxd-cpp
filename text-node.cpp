#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "text-node.h"
#include "document-internal.h"

TextNode::TextNode(document_t *doc, const char *text) :
  Node(doc, NODE_TYPE_TEXT_NODE),
  text_(document_intern(doc, text))
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
TextNode::output(output_t *output)
{
  output->fn(output->data, "%s", text_);
}

void
TextNode::change_document(document_t *doc)
{
  Node::change_document(doc);
  text_ = document_intern(doc, text_);
}
