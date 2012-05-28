#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "text-node.h"
#include "node-internal.h"
#include "document-internal.h"

void
text_node_free(text_node_t *tn)
{
  delete tn;
}

node_t *
text_node_cast_to_node(text_node_t *tn)
{
  return tn;
}

text_node_t *
text_node_new(document_t *doc, const char * const text)
{
  return new TextNode(doc, text);
}

const char *
text_node_text(text_node_t *tn)
{
  return tn->text();
}

void
text_node_output(text_node_t *tn, output_t *output)
{
  tn->output(output);
}

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
