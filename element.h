#ifndef ELEMENT_H
#define ELEMENT_H

#include "node.h"
#include <map>

class Element : public Node {
public:
  Element(Document *doc, const char * const name);

  const char *name() const;
  void output(Output &output) const;
  void set_attribute(const char * const name, const char * const value);
  const char *get_attribute(const char * const name);

  std::ostream& to_stream(std::ostream& os) const;

private:
  std::string _name;
  std::map<std::string, std::string> _attributes;
};

#endif
