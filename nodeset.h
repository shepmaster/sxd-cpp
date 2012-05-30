#ifndef NODESET_H
#define NODESET_H

typedef struct nodesetS nodeset_t;

#include "node.h"

void
nodeset_free(nodeset_t *nodeset);

nodeset_t *
nodeset_new(void);

unsigned int
nodeset_count(nodeset_t *nodeset);

void
nodeset_add(nodeset_t *nodeset, Node *node);

void
nodeset_add_nodeset(nodeset_t *nodeset, nodeset_t *to_add);

Node *
nodeset_get(nodeset_t *nodeset, unsigned int i);

#endif
