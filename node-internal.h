#ifndef NODE_INTERNAL_H
#define NODE_INTERNAL_H

#include "document.h"
#include "node.h"

typedef void (*change_document_t)(node_t *node, document_t *doc);

struct nodeS {
  document_t *doc;
  struct nodeS *parent;
  GList *children;

  struct {
    change_document_t change_document;
  } fn;
};

void
node_destroy(node_t *node);

node_t *
node_new(document_t *doc, const char * const name);

void
node_change_document(node_t *node, document_t *doc);

#endif
