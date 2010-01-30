#ifndef NODE_H
#define NODE_H

typedef struct nodeS node_t;

#include "document.h"

void
node_free(node_t *node);

node_t *
node_cast_to_node(node_t *n);

const char *
node_name(node_t *node);

void
node_set_attribute(node_t *node, const char * const name, const char * const value);

const char *
node_get_attribute(node_t *node, const char * const name);

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
