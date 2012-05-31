#ifndef TEXT_NODE_H
#define TEXT_NODE_H

#include "node.h"

class TextNode : public Node {
public:
  TextNode(document_t *doc, const char *text);
  ~TextNode();
  const char *text();
  void output(Output &output);
  void change_document(document_t *doc);
private:

  const char *text_;
};

#endif
