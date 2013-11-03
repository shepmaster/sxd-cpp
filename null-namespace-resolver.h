#ifndef NULL_NAMESPACE_RESOLVER_H
#define NULL_NAMESPACE_RESOLVER_H

#include "namespace-resolver.h"

class NullNamespaceResolver : public NamespaceResolver {
  const std::string *
  find_namespace_for_prefix(const std::string prefix) const {
    return nullptr;
  };
};

#endif
