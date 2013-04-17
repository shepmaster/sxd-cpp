#ifndef ELEMENT_H
#define ELEMENT_H

#include "node.h"
#include "attribute.h"
#include "qname.h"
#include <map>

class Element : public Node {
public:
  Element(Document *doc, const QName qname);

  const std::string name() const;
  const QName qname() const;

  void output(Output &output) const;
  Attribute *set_attribute(const std::string name, const std::string value);
  const std::string get_attribute(const std::string name);

  void foreach_attribute(foreach_fn_t fn) const;

  std::ostream& to_stream(std::ostream& os) const;

private:
  QName _qname;
  std::map<std::string, Attribute *> _attributes;
};

#endif
