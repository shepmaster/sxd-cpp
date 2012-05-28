#ifndef COMMENT_H
#define COMMENT_H

typedef class Comment comment_t;

#include "document.h"

const char *
comment_text(comment_t *comment);

node_t *
comment_cast_to_node(comment_t *c);

void
comment_free(comment_t *comment);

comment_t *
comment_new(document_t *doc, const char * const text);

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
