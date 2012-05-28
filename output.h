#ifndef OUTPUT_H
#define OUTPUT_H

typedef void (*output_fn_t)(void *data, const char * format, ...);

typedef struct {
  output_fn_t fn;
  void *data;
} output_t;

#endif
