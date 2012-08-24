#ifndef XPATH_TOKEN_H
#define XPATH_TOKEN_H

#include <string>

enum class XPathTokenType {
  String,
  DoubleColon
};

class XPathToken {
public:
  XPathToken(std::string string);
  XPathToken(XPathTokenType type);

  std::string string();
  bool is(XPathTokenType type);

private:
  XPathTokenType _type;
  std::string _string;
};

#endif
