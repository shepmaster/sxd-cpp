#ifndef COMMENT_H
#define COMMENT_H

#include <string>
#include "node.h"

class Comment : public Node {
public:
  Comment(Document *doc, const char * const text);
  ~Comment();

  const char *text();
  void output(Output &output) const;
private:
  std::string text_;
};

#endif
