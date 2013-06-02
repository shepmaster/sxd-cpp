#ifndef PREFIXED_NAME_H
#define PREFIXED_NAME_H

#include <string>

class PrefixedName
{
public:
  PrefixedName(const std::string name);
  PrefixedName(const std::string namespace_prefix, const std::string name);

  const bool has_prefix() const;
  const std::string name() const;

  bool operator==(const PrefixedName &other) const;

private:
  bool _has_prefix;
  std::string _name;
};

#endif
