#ifndef ELEMENT_H
#define ELEMENT_H

#include "node.h"
#include "attribute.h"
#include <map>

class Element : public Node {
public:
  Element(Document *doc, const std::string name);

  const std::string name() const;
  void output(Output &output) const;
  Attribute *set_attribute(const std::string name, const std::string value);
  const std::string get_attribute(const std::string name);

  std::ostream& to_stream(std::ostream& os) const;

private:
  std::string _name;
  std::map<std::string, Attribute *> _attributes;
};

#endif
