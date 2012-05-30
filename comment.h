#ifndef COMMENT_H
#define COMMENT_H

#include "document.h"

class Comment : public Node {
public:
  Comment(document_t *doc, const char * const text);
  ~Comment();

  const char *text();
  void output(output_t *output);
private:
  char *text_;
};

#endif
