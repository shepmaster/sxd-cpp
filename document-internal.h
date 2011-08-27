#ifndef DOCUMENT_INTERNAL_H
#define DOCUMENT_INTERNAL_H

#include "document.h"
#include "tokenizer.h"

const char *
document_intern(document_t *doc, const char * const string);

void
document_stop_managing_node(document_t *doc, node_t *node);

void
parse_preamble(tokenizer_t *tokenizer, GError **error);

element_t *
parse_element(document_t *document, tokenizer_t *tokenizer, GError **error);

void
parse_attribute(tokenizer_t *tokenizer, element_t *element, GError **error);

#endif
