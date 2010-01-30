#ifndef ELEMENT_INTERNAL_H
#define ELEMENT_INTERNAL_H

#include "element.h"

element_t *
element_new(document_t *doc, const char * const name);

void
element_change_document(node_t *n, document_t *doc);

#endif
