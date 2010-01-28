#ifndef INTERN_H
#define INTERN_H

typedef struct internS intern_t;

void
intern_free(intern_t *i);

intern_t *
intern_new(void);

const char *
intern_intern(intern_t *i, const char * const string);

#endif
