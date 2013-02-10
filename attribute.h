#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "node.h"

class Attribute : public Node {
public:
  Attribute(Document *doc, const std::string name, const std::string value);

  const std::string name() const;
  const std::string value() const;

  void output(Output &output) const;

  std::ostream& to_stream(std::ostream& os) const;

private:
  std::string _name;
  std::string _value;
};

#endif
