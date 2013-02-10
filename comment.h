#ifndef COMMENT_H
#define COMMENT_H

#include <string>
#include "node.h"

class Comment : public Node {
public:
  Comment(Document *doc, const std::string text);

  const std::string text() const;
  void output(Output &output) const;
private:
  std::string _text;
};

#endif
