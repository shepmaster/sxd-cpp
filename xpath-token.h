#ifndef XPATH_TOKEN_H
#define XPATH_TOKEN_H

#include <iostream>
#include <string>

enum class XPathTokenType {
  String,
  NodeTest,
  Function,
  Axis,
  Literal,
  Number,
  CurrentNode,
  ParentNode,
  PlusSign,
  MinusSign,
  Equal,
  And,
  Or,
  Remainder,
  Divide,
  Multiply,
  LeftParen,
  RightParen,
  LeftBracket,
  RightBracket,
  AtSign,
  DoubleColon,
  Slash,
  DoubleSlash
};

class XPathToken {
public:
  XPathToken(std::string string);
  XPathToken(double number);
  XPathToken(XPathTokenType type);
  XPathToken(XPathTokenType type, std::string string);

  std::string string() const;
  double number() const;
  bool is(XPathTokenType type) const;

  bool precedes_node_test() const;
  bool precedes_expression() const;
  bool is_operator() const;

  bool operator==(const XPathToken &other) const;

private:
  XPathTokenType _type;
  std::string _string;
  double _number;

  friend std::ostream& operator<<(std::ostream&, const XPathToken&);
};

std::ostream& operator<<(std::ostream &, const XPathTokenType &);
std::ostream& operator<<(std::ostream &, const XPathToken &);

#endif
