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

void
nodeset_add_nodeset(nodeset_t *nodeset, nodeset_t *to_add)
{
  GPtrArray *g_nodeset = (GPtrArray *)nodeset;
  GPtrArray *g_to_add = (GPtrArray *)to_add;
  int i;

  for (i = 0; i < g_to_add->len; i++) {
    g_ptr_array_add(g_nodeset, g_ptr_array_index(g_to_add, i));
  }
}

node_t *
nodeset_get(nodeset_t *nodeset, unsigned int i)
{
  GPtrArray *garray = (GPtrArray *)nodeset;
  return (node_t *)garray->pdata[i];
}
