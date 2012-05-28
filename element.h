#ifndef ELEMENT_H
#define ELEMENT_H

typedef class Element element_t;

#include "node.h"
#include "output.h"

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

class Element : public Node {
public:
  Element(document_t *doc, const char * const name);
  ~Element();

  const char *name();
  void output(output_t *output);
  void set_attribute(const char * const name, const char * const value);
  const char *get_attribute(const char * const name);
  void change_document(document_t *doc);

private:
  const char * name_;
  GHashTable *attributes;
};

#endif
