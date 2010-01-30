#ifndef ELEMENT_H
#define ELEMENT_H

typedef struct elementS element_t;

#include "node.h"

void
element_free(element_t *e);

node_t *
element_cast_to_node(element_t *e);

const char *
element_name(element_t *e);

void
element_set_attribute(element_t *e, const char * const name, const char * const value);

const char *
element_get_attribute(element_t *e, const char * const name);

#endif
