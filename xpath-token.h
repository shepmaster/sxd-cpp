#ifndef XPATH_TOKEN_H
#define XPATH_TOKEN_H

#include "prefixed-name.h"

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
  DollarSign,
  PlusSign,
  MinusSign,
  Pipe,
  Equal,
  NotEqual,
  LessThan,
  LessThanOrEqual,
  GreaterThan,
  GreaterThanOrEqual,
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
  XPathToken(PrefixedName prefixed_name);
  XPathToken(double number);
  XPathToken(XPathTokenType type);
  XPathToken(XPathTokenType type, std::string string);

  std::string string() const;
  PrefixedName prefixed_name() const;
  double number() const;
  bool is(XPathTokenType type) const;

  bool precedes_node_test() const;
  bool precedes_expression() const;
  bool is_operator() const;

  bool operator==(const XPathToken &other) const;

private:
  XPathTokenType _type;
  PrefixedName _prefixed_name;
  std::string _string;
  double _number;

  friend std::ostream& operator<<(std::ostream&, const XPathToken&);
};

std::ostream& operator<<(std::ostream &, const XPathTokenType &);
std::ostream& operator<<(std::ostream &, const XPathToken &);

#endif
