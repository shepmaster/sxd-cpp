#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nmtoken.h"

struct nmtokenS {
  char *namespace;
  char *name;
};

void
nmtoken_free(nmtoken_t *nm)
{
  free(nm->namespace);
  free(nm->name);
  free(nm);
}

nmtoken_t *
nmtoken_new(const char * const name)
{
  nmtoken_t *nm;
  const char * colon;

  nm = calloc(1, sizeof(*nm));
  colon = strchr(name, ':');

  if (colon) {
    char *modified = strdup(name);
    modified[colon - name] = '\0';
    nm->namespace = modified;
    nm->name = strdup(colon+1);
  } else {
    nm->name = strdup(name);
  }

  return nm;
}

const char *
nmtoken_name(nmtoken_t *nm)
{
  return nm->name;
}

const char *
nmtoken_namespace(nmtoken_t *nm)
{
  return nm->namespace;
}
