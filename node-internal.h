#ifndef NODE_INTERNAL_H
#define NODE_INTERNAL_H

#include <glib.h>
#include "document.h"
#include "node.h"

typedef void (*free_node_t)(node_t *node);
typedef void (*change_document_t)(node_t *node, document_t *doc);

struct nodeS {
  document_t *doc;
  struct nodeS *parent;
  GList *children;

  struct {
    free_node_t free_node;
    change_document_t change_document;
  } fn;
};

void
node_destroy(node_t *node);

void
node_init(node_t *node, document_t *doc);

void
node_change_document(node_t *node, document_t *doc);

#endif
