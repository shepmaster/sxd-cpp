#ifndef TEXT_NODE_H
#define TEXT_NODE_H

typedef struct text_nodeS text_node_t;

#include "document.h"

void
text_node_free(text_node_t *tn);

node_t *
text_node_cast_to_node(text_node_t *tn);

text_node_t *
text_node_new(document_t *doc, const char * const text);

const char *
text_node_text(text_node_t *tn);

void
text_node_output(text_node_t *tn, output_t *output);

#endif
