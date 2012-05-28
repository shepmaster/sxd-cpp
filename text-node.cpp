#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "text-node.h"
#include "node-internal.h"
#include "document-internal.h"

struct text_nodeS {
  node_t node;
  TextNode *tn;
};

void
text_node_free(text_node_t *tn)
{
  if (! tn) return;
  delete tn->tn;
  free(tn);
}

node_t *
text_node_cast_to_node(text_node_t *tn)
{
  return (node_t *)tn;
}

static void
text_node_free_node(node_t *node)
{
  text_node_free((text_node_t *)node);
}

static void
text_node_change_document(node_t *node, document_t *doc)
{
  text_node_t *tn = (text_node_t *)node;
  tn->tn->change_document(doc);
}

text_node_t *
text_node_new(document_t *doc, const char * const text)
{
  text_node_t *tn;

  tn = (text_node_t *)calloc(1, sizeof(*tn));
  tn->tn = new TextNode(&tn->node, doc, text);
  return tn;
}

const char *
text_node_text(text_node_t *tn)
{
  return tn->tn->text();
}

void
text_node_output(text_node_t *tn, output_t *output)
{
  tn->tn->output(output);
}

TextNode::TextNode(node_t *node, document_t *doc, const char *text) :
  node(node), text_(document_intern(doc, text))
{
  node_init(node, doc);
  node->type = NODE_TYPE_TEXT_NODE;
  node->fn.free_node = text_node_free_node;
  node->fn.change_document = text_node_change_document;
}

TextNode::~TextNode()
{
  node_destroy(node);
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
  text_ = document_intern(doc, text_);
}
