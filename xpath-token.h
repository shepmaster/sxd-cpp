#ifndef XPATH_TOKEN_H
#define XPATH_TOKEN_H

#include <iostream>
#include <string>

enum class XPathTokenType {
  String,
  LeftParen,
  RightParen,
  DoubleColon,
  DoubleSlash
};

class XPathToken {
public:
  XPathToken(std::string string);
  XPathToken(XPathTokenType type);

  std::string string() const;
  bool is(XPathTokenType type) const;

private:
  XPathTokenType _type;
  std::string _string;

  friend std::ostream& operator<<(std::ostream&, const XPathToken&);
};

std::ostream& operator<<(std::ostream &, const XPathTokenType &);
std::ostream& operator<<(std::ostream &, const XPathToken &);

#endif
