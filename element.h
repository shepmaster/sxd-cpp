#ifndef ELEMENT_H
#define ELEMENT_H

#include <glib.h>
#include "node.h"

class Element : public Node {
public:
  Element(Document *doc, const char * const name);
  ~Element();

  const char *name() const;
  void output(Output &output);
  void set_attribute(const char * const name, const char * const value);
  const char *get_attribute(const char * const name);
  void change_document(Document *doc);

  std::ostream& to_stream(std::ostream& os) const;

private:
  std::string _name;
  GHashTable *attributes;
};

#endif
