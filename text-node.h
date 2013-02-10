#ifndef TEXT_NODE_H
#define TEXT_NODE_H

#include "node.h"

class TextNode : public Node {
public:
  TextNode(Document *doc, const std::string text);
  const std::string text() const;
  void output(Output &output) const;

  std::ostream& to_stream(std::ostream& os) const;

private:
  const std::string _text;
};

#endif
