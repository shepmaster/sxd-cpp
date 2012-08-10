#include <iostream>
#include <stdlib.h>
#include <glib.h>

#include "intern.h"

#define STRING_CHUNK_SIZE 1024

#define internS GStringChunk

void
intern_free(intern_t *i)
{
  g_string_chunk_free((GStringChunk *)i);
}

intern_t *
intern_new(void)
{
  return (intern_t *)g_string_chunk_new(STRING_CHUNK_SIZE);
}

const char *
intern_intern(intern_t *i, const char * const string)
{
  return g_string_chunk_insert_const((GStringChunk *)i, string);
}
