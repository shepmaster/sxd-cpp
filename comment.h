#ifndef COMMENT_H
#define COMMENT_H

typedef struct commentS comment_t;

#include "document.h"

const char *
comment_text(comment_t *comment);

void
comment_free(comment_t *comment);

comment_t *
comment_new(document_t *doc, const char * const text);

#endif
