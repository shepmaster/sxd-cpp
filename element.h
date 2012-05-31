#ifndef ELEMENT_H
#define ELEMENT_H

#include "node.h"

class Element : public Node {
public:
  Element(Document *doc, const char * const name);
  ~Element();

  const char *name();
  void output(Output &output);
  void set_attribute(const char * const name, const char * const value);
  const char *get_attribute(const char * const name);
  void change_document(Document *doc);

private:
  const char * name_;
  GHashTable *attributes;
};

#endif
