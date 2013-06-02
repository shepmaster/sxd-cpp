#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "node.h"
#include "namespace-resolver.h"

class Element;

class Attribute : public Node, NamespaceResolver {
public:
  Attribute(Document *doc, const Element &element,
            const std::string name, const std::string value);

  const std::string name() const;
  const std::string value() const;

  const std::string *find_namespace_for_prefix(const std::string prefix) const;

  void output(Output &output) const;

  std::ostream& to_stream(std::ostream& os) const;

private:
  const Element &_element;
  std::string _name;
  std::string _value;
};

#endif
