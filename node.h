#ifndef NODE_H
#define NODE_H

typedef struct nodeS node_t;

#include "document.h"

void
node_free(node_t *node);

void
node_append_child(node_t *node, node_t *child);

void
node_remove_child(node_t *node, node_t *child);

node_t *
node_first_child(node_t *node);

node_t *
node_parent(node_t *node);

node_t *
node_next_sibling(node_t *node);

document_t *
node_document(node_t *node);

#endif
