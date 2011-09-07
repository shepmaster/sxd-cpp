#ifndef NODE_H
#define NODE_H

typedef struct nodeS node_t;
typedef enum {
  NODE_TYPE_ELEMENT,
  NODE_TYPE_TEXT_NODE,
  NODE_TYPE_COMMENT
} node_type_t;

#include "document.h"
#include "nodeset.h"

void
node_free(node_t *node);

node_type_t
node_type(node_t *node);

void
node_append_child(node_t *node, node_t *child);

void
node_remove_child(node_t *node, node_t *child);

node_t *
node_first_child(node_t *node);

node_t *
node_parent(node_t *node);

node_t *
node_prev_sibling(node_t *node);

node_t *
node_next_sibling(node_t *node);

void
node_insert_next_sibling(node_t *node, node_t *sibling);

document_t *
node_document(node_t *node);

#endif
