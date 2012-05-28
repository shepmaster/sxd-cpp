#ifndef ELEMENT_H
#define ELEMENT_H

typedef struct elementS element_t;

typedef void (*output_fn_t)(void *data, const char * format, ...);
typedef struct {
  output_fn_t fn;
  void *data;
} output_t;

#include "node.h"

void
element_free(element_t *e);

node_t *
element_cast_to_node(element_t *e);

const char *
element_name(element_t *e);

void
element_set_attribute(element_t *e, const char * const name, const char * const value);

const char *
element_get_attribute(element_t *e, const char * const name);

void
element_output(element_t *element, output_t *output);

class Element {
public:
  Element(document_t *doc, node_t *node, const char * const name);
  ~Element();

  const char *name();
  void output(output_t *output);
  void set_attribute(const char * const name, const char * const value);
  const char *get_attribute(const char * const name);
  void change_document(document_t *doc);

private:
  node_t *node;
  const char * name_;
  GHashTable *attributes;
};

#endif
