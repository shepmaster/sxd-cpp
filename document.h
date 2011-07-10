#ifndef DOCUMENT_H
#define DOCUMENT_H

typedef struct documentS document_t;

#include "node.h"
#include "element.h"
#include "text-node.h"

void
document_free(document_t *doc);

document_t *
document_new(void);

element_t *
document_element_new(document_t *doc, const char * const name);

text_node_t *
document_text_node_new(document_t *doc, const char * const text);

void
document_manage_node(document_t *doc, node_t *node);

unsigned int
document_managed_node_count(document_t *doc);

document_t *
document_parse(const char *input);

element_t *
document_root(document_t *doc);

#endif
