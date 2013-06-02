#ifndef PREFIXED_NAME_H
#define PREFIXED_NAME_H

#include <string>

class PrefixedName
{
public:
  PrefixedName(const std::string name);
  PrefixedName(const std::string namespace_prefix, const std::string name);

  const std::string name() const;

  bool operator==(const PrefixedName &other) const;

private:
  std::string _name;
};

#endif
