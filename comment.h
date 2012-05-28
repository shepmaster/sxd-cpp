#ifndef COMMENT_H
#define COMMENT_H

typedef struct commentS comment_t;

#include "document.h"

const char *
comment_text(comment_t *comment);

node_t *
comment_cast_to_node(comment_t *c);

void
comment_free(comment_t *comment);

comment_t *
comment_new(document_t *doc, const char * const text);

class Comment {
public:
  Comment(node_t *node, document_t *doc, const char * const text);

  Comment(document_t *doc, const char * const text);
  ~Comment();

  const char *text();
private:
  node_t *node;
  char *text_;
};

#endif
