#ifndef NODE_H
#define NODE_H

typedef struct nodeS node_t;
typedef enum {
  NODE_TYPE_ELEMENT,
  NODE_TYPE_TEXT_NODE
} node_type_t;
typedef enum {
  XPATH_PREDICATE_ELEMENT = 1 << 0,
  XPATH_PREDICATE_TEXT_NODE = 1 << 1
} xpath_predicate_type_t;

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
node_next_sibling(node_t *node);

document_t *
node_document(node_t *node);

nodeset_t *
node_select_xpath(node_t *node, xpath_predicate_type_t select, const char * const name);

#endif
