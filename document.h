#ifndef DOCUMENT_H
#define DOCUMENT_H

typedef struct documentS document_t;

#include <glib.h>

#include "node.h"
#include "element.h"
#include "text-node.h"
#include "comment.h"

void
document_free(document_t *doc);

document_t *
document_new(void);

Element *
document_element_new(document_t *doc, const char * const name);

text_node_t *
document_text_node_new(document_t *doc, const char * const text);

Comment *
document_comment_new(document_t *doc, const char * const text);

void
document_manage_node(document_t *doc, node_t *node);

unsigned int
document_managed_node_count(document_t *doc);

#define DOCUMENT_PARSE_ERROR document_parse_error_quark()

GQuark
document_parse_error_quark(void);

typedef enum {
  DOCUMENT_PARSE_ERROR_FAILED
} DocumentParseError;

document_t *
document_parse(const char *input, GError **error);

Element *
document_root(document_t *doc);

#endif
