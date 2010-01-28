#ifndef NMTOKEN_H
#define NMTOKEN_H

typedef struct nmtokenS nmtoken_t;

void
nmtoken_free(nmtoken_t *nm);

nmtoken_t *
nmtoken_new(const char * const name);

const char *
nmtoken_name(nmtoken_t *nm);

const char *
nmtoken_namespace(nmtoken_t *nm);

#endif
