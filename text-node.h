#ifndef TEXT_NODE_H
#define TEXT_NODE_H

typedef class TextNode text_node_t;

#include "document.h"

class TextNode : public Node {
public:
  TextNode(document_t *doc, const char *text);
  ~TextNode();
  const char *text();
  void output(output_t *output);
  void change_document(document_t *doc);
private:

  const char *text_;
};

#endif
