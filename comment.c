#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comment.h"

struct commentS {
  char *text;
};

const char *
comment_text(comment_t *c)
{
  return c->text;
}

void
comment_free(comment_t *c)
{
  free(c->text);
  free(c);
}

comment_t *
comment_new(document_t *doc, const char * const text)
{
  comment_t *c;

  c = calloc(sizeof(*c), 1);
  c->text = strdup(text);

  return c;
}
