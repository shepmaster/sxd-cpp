#ifndef TEXT_NODE_H
#define TEXT_NODE_H

#include "node.h"

class TextNode : public Node {
public:
  TextNode(Document *doc, const char *text);
  const char *text();
  void output(Output &output) const;
private:

  const std::string _text;
};

#endif
