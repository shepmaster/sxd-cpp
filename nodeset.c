#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "nodeset.h"

#define nodesetS GPtrArray

void
nodeset_free(nodeset_t *nodeset)
{
  g_ptr_array_free((GPtrArray *)nodeset, TRUE);
}

nodeset_t *
nodeset_new(void)
{
  return (nodeset_t *)g_ptr_array_new();
}

unsigned int
nodeset_count(nodeset_t *nodeset)
{
  GPtrArray *garray = (GPtrArray *)nodeset;
  return garray->len;
}

void
nodeset_add(nodeset_t *nodeset, node_t *node)
{
  g_ptr_array_add((GPtrArray *)nodeset, node);
}

node_t *
nodeset_get(nodeset_t *nodeset, unsigned int i)
{
  GPtrArray *garray = (GPtrArray *)nodeset;
  return garray->pdata[i];
}
