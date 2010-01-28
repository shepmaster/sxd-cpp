#ifndef NODE_INTERNAL_H
#define NODE_INTERNAL_H

#include "node.h"

node_t *
node_new(document_t *doc, const char * const name);

void
node_change_document(node_t *node, document_t *doc);

#endif
