#ifndef PREFIXED_NAME_H
#define PREFIXED_NAME_H

#include <string>

class PrefixedName
{
public:
  PrefixedName(const std::string name);
  PrefixedName(const std::string prefix, const std::string name);

  const bool has_prefix() const;
  const std::string prefix() const;
  const std::string name() const;

  bool operator==(const PrefixedName &other) const;
  bool operator!=(const PrefixedName &other) const;

private:
  bool _has_prefix;
  std::string _prefix;
  std::string _name;

  friend std::ostream &operator<<(std::ostream &os, const PrefixedName &obj);
};

std::ostream &operator<<(std::ostream &os, const PrefixedName &obj);

#endif
