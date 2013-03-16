#ifndef XPATH_VALUE_H
#define XPATH_VALUE_H

#include "nodeset.h"

#include <string>
#include <iosfwd>

class XPathValueImpl;

class XPathValue
{
public:
  enum class Type {
    Number,
    String,
    Boolean,
    Nodeset
  };

  XPathValue(double value);
  XPathValue(std::string value);
  XPathValue(const char *value);
  XPathValue(bool value);
  XPathValue(Nodeset value);

  double number() const;
  std::string string() const;
  bool boolean() const;
  Nodeset nodeset() const;

  bool is(Type type) const;

  bool operator==(const XPathValue &other) const;

private:
  std::shared_ptr<XPathValueImpl> _impl;

  friend std::ostream& operator<<(std::ostream&, const XPathValue&);
};

std::ostream& operator<<(std::ostream &, const XPathValue::Type &);
std::ostream &operator<<(std::ostream &, const XPathValue &);

class XPathValueImpl : public ToStream
{
public:
  virtual double number() const = 0;
  virtual std::string string() const = 0;
  virtual bool boolean() const = 0;
  virtual Nodeset nodeset() const = 0;
  virtual bool is(XPathValue::Type type) const = 0;
  virtual bool operator==(const XPathValueImpl &other) const = 0;
};

#endif
