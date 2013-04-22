#ifndef NAMESPACE_RESOLVER_H
#define NAMESPACE_RESOLVER_H

class NamespaceResolver
{
public:
  virtual ~NamespaceResolver() {};

  virtual const std::string *
  find_namespace_for_prefix(const std::string prefix) const = 0;
};

#endif
