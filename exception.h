#ifndef EXCEPTION_H
#define EXCEPTION_H

#define FALSE 0
#define TRUE !FALSE

#define EX_EXCEPTION_ID 0
#define EX_INVALID_EXCEPTION_ID 1
#define EX_INVALID_VALUE_ID 2
#define EX_INVALID_STRING_VALUE_ID 3

typedef struct exceptionS exception_t;

void
exception_free(exception_t *ex);

exception_t *
exception_new(int id);

int
exception_is_a(exception_t *ex, int id);

#endif
