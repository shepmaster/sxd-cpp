#ifndef XPATH_TOKEN_H
#define XPATH_TOKEN_H

#include <iostream>
#include <string>

enum class XPathTokenType {
  String,
  Number,
  Apostrophe,
  LeftParen,
  RightParen,
  LeftBracket,
  RightBracket,
  AtSign,
  DoubleColon,
  DoubleSlash
};

class XPathToken {
public:
  XPathToken(std::string string);
  XPathToken(double number);
  XPathToken(XPathTokenType type);

  std::string string() const;
  double number() const;
  bool is(XPathTokenType type) const;

private:
  XPathTokenType _type;
  std::string _string;
  double _number;

  friend std::ostream& operator<<(std::ostream&, const XPathToken&);
};

std::ostream& operator<<(std::ostream &, const XPathTokenType &);
std::ostream& operator<<(std::ostream &, const XPathToken &);

#endif
