#ifndef DOCUMENT_INTERNAL_H
#define DOCUMENT_INTERNAL_H

#include "document.h"
#include "tokenizer.h"

const char *
document_intern(document_t *doc, const char * const string);

void
document_stop_managing_node(document_t *doc, node_t *node);

#endif
