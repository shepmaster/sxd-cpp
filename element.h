#ifndef ELEMENT_H
#define ELEMENT_H

#include "node.h"
#include "output.h"

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
