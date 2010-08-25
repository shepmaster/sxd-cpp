#ifndef NODE_INTERNAL_H
#define NODE_INTERNAL_H

#include <glib.h>
#include "document.h"
#include "node.h"

typedef void (*free_node_t)(node_t *node);
typedef void (*change_document_t)(node_t *node, document_t *doc);

typedef void (*node_foreach_fn_t)(node_t *node, gpointer user_data);

struct nodeS {
  node_type_t type;
  document_t *doc;
  node_t *parent;
  node_t *first_child;
  node_t *prev_sibling;
  node_t *next_sibling;

  struct {
    free_node_t free_node;
    change_document_t change_document;
  } fn;
};

void
node_foreach_child(node_t *node, node_foreach_fn_t fn, gpointer user_data);

void
node_foreach_ancestor(node_t *node, node_foreach_fn_t fn, gpointer user_data);

void
node_destroy(node_t *node);

void
node_init(node_t *node, document_t *doc);

void
node_change_document(node_t *node, document_t *doc);

void
node_output_children(node_t *node, output_t *output);

#endif
