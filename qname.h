#ifndef QNAME_H
#define QNAME_H

#include <string>

class QName
{
public:
  QName(const std::string name);
  QName(const std::string namespace_uri, const std::string name);

  const bool has_namespace() const;
  const std::string name() const;

  bool operator==(const QName &other) const;
  bool operator!=(const QName &other) const;

private:
  bool _has_namespace;
  std::string _namespace_uri;
  std::string _name;

  friend std::ostream &operator<<(std::ostream &os, const QName &obj);
};

std::ostream &operator<<(std::ostream &os, const QName &obj);

#endif
