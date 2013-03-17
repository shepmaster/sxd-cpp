#include "xpath-parser.h"

#include "axis-child.h"
#include "axis-self.h"
#include "axis-parent.h"
#include "axis-descendant.h"
#include "axis-descendant-or-self.h"
#include "axis-attribute.h"
#include "node-test-node.h"
#include "node-test-element.h"
#include "node-test-attribute.h"
#include "node-test-text.h"
#include "expression-literal.h"
#include "expression-variable.h"
#include "expression-function.h"
#include "expression-path.h"
#include "expression-math.h"
#include "expression-negation.h"
#include "expression-or.h"
#include "expression-and.h"
#include "expression-equal.h"
#include "expression-relational.h"
#include "make-unique.h"
#include "xpath-parsing-exceptions.h"

using std::move;

using ParseFn = std::unique_ptr<XPathExpression>(*)(XPathTokenSource &source);

XPathParser::XPathParser(XPathTokenSource &source) :
  _source(source)
{
}

void
consume(XPathTokenSource &source, XPathTokenType type) {
  auto token = source.next_token();
  if (! token.is(type)) {
    throw UnexpectedTokenException();
  }
}

std::unique_ptr<XPathAxis>
parse_axis(XPathTokenSource &source) {
  if (source.next_token_is(XPathTokenType::Axis)) {
    auto token = source.next_token();
    auto name = token.string();
    consume(source, XPathTokenType::DoubleColon);

    if (name == "self") {
      return make_unique<AxisSelf>();
    } else if (name == "parent") {
      return make_unique<AxisParent>();
    } else if (name == "descendant") {
      return make_unique<AxisDescendant>();
    } else if (name == "descendant-or-self") {
      return make_unique<AxisDescendantOrSelf>();
    } else if (name == "attribute") {
      return make_unique<AxisAttribute>();
    } else {
      throw InvalidXPathAxisException(name);
    }
  }

  return make_unique<AxisChild>();
}

std::unique_ptr<XPathNodeTest>
parse_node_test(XPathTokenSource &source) {
  if (source.next_token_is(XPathTokenType::NodeTest)) {
    auto token = source.next_token();
    auto name = token.string();

    consume(source, XPathTokenType::LeftParen);
    consume(source, XPathTokenType::RightParen);

    if (name == "node") {
      return make_unique<NodeTestNode>();
    } else if (name == "text") {
      return make_unique<NodeTestText>();
    } else {
      throw InvalidNodeTestException(name);
    }
  }

  return nullptr;
}

std::unique_ptr<XPathNodeTest>
default_node_test(XPathTokenSource &source, std::unique_ptr<XPathAxis> &axis) {
  if (source.next_token_is(XPathTokenType::String)) {
    auto token = source.next_token();

    switch (axis->principal_node_type()) {
    case PrincipalNodeType::Attribute:
      return make_unique<NodeTestAttribute>(token.string());
    case PrincipalNodeType::Element:
      return make_unique<NodeTestElement>(token.string());
    }
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_variable_reference(XPathTokenSource &source)
{
  if (source.next_token_is(XPathTokenType::DollarSign)) {
    consume(source, XPathTokenType::DollarSign);
    auto token = source.next_token();
    // TODO: check is a string

    return make_unique<ExpressionVariable>(token.string());
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_string_literal(XPathTokenSource &source) {
  if (source.next_token_is(XPathTokenType::Literal)) {
    auto token = source.next_token();
    return make_unique<ExpressionLiteral>(token.string());
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_numeric_literal(XPathTokenSource &source)
{
  if (source.next_token_is(XPathTokenType::Number)) {
    auto token = source.next_token();
    return make_unique<ExpressionLiteral>(token.number());
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_primary_expression(XPathTokenSource &source);

std::unique_ptr<XPathExpression>
parse_function_call(XPathTokenSource &source)
{
  if (source.next_token_is(XPathTokenType::Function)) {
    auto token = source.next_token();

    std::vector<std::shared_ptr<XPathExpression>> arguments;

    consume(source, XPathTokenType::LeftParen);
    while (! source.next_token_is(XPathTokenType::RightParen)) {
      // TODO: this should be the top-level expression
      auto arg = parse_primary_expression(source);
      if (! arg) break;
      arguments.push_back(move(arg));
    }
    consume(source, XPathTokenType::RightParen);

    return make_unique<ExpressionFunction>(token.string(), std::move(arguments));
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_children_in_order(std::vector<ParseFn> child_parses, XPathTokenSource &source)
{
  for (auto child_parse : child_parses) {
    auto expr = child_parse(source);
    if (expr) return expr;
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_primary_expression(XPathTokenSource &source) {
  std::vector<ParseFn> child_parses = {
    parse_variable_reference,
    parse_string_literal,
    parse_numeric_literal,
    parse_function_call
  };

  return parse_children_in_order(child_parses, source);
}

std::unique_ptr<XPathStep>
parse_step(XPathTokenSource &source)
{
  auto axis = parse_axis(source);

  auto node_test = parse_node_test(source);
  if (! node_test) {
    node_test = default_node_test(source, axis);
  }
  if (! node_test) {
    return nullptr;
  }

  std::unique_ptr<XPathExpression> predicate;
  if (source.next_token_is(XPathTokenType::LeftBracket)) {
    consume(source, XPathTokenType::LeftBracket);

    predicate = parse_primary_expression(source);
    if (! predicate) {
      throw EmptyPredicateException();
    }

    consume(source, XPathTokenType::RightBracket);
  }

  return make_unique<XPathStep>(move(axis), move(node_test), move(predicate));
}

std::unique_ptr<XPathExpression>
parse_relative_location_path(XPathTokenSource &source)
{
  std::vector<std::unique_ptr<XPathStep>> steps;

  auto step = parse_step(source);
  if (step) {
    steps.push_back(move(step));
    while (source.next_token_is(XPathTokenType::Slash)) {
      consume(source, XPathTokenType::Slash);
      auto next = parse_step(source);
      steps.push_back(move(next));
    }

    return make_unique<ExpressionPath>(move(steps));
  }

  // expr = parse_abbreviated_relative_location_path();
  // if (expr) return expr;

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_location_path(XPathTokenSource &source)
{
  auto expr = parse_relative_location_path(source);
  if (expr) return expr;

  // expr = parse_absolute_location_path(source);
  // if (expr) return expr;

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_path_expression(XPathTokenSource &source)
{
  std::vector<ParseFn> child_parses = {
    parse_location_path,
    parse_primary_expression
  };

  return parse_children_in_order(child_parses, source);
}

std::unique_ptr<XPathExpression>
parse_unary_expression(XPathTokenSource &source)
{
  auto expr = parse_path_expression(source);
  if (expr) return expr;

  if (source.next_token_is(XPathTokenType::MinusSign)) {
    consume(source, XPathTokenType::MinusSign);
    expr = parse_unary_expression(source);
    return make_unique<ExpressionNegation>(move(expr));
  }

  return nullptr;
}

using SubExpression = std::shared_ptr<XPathExpression>;

template<class T>
using BinaryExpressionBuilder =
  std::unique_ptr<T>(*)(SubExpression left, SubExpression right);

template<class T>
struct BinaryRule {
  XPathTokenType type;
  BinaryExpressionBuilder<T> builder;
};

template<class T>
class LeftAssociativeBinaryParser {
public:
  LeftAssociativeBinaryParser(ParseFn child, std::vector<BinaryRule<T>> rules) :
    child_parse(child), rules(rules)
  {}

  std::unique_ptr<XPathExpression>
  parse(XPathTokenSource &source) {
    auto left = child_parse(source);
    if (! left) return nullptr;

    while (source.has_more_tokens()) {
      bool found = false;

      for (auto rule : rules) {
        if (source.next_token_is(rule.type)) {
          consume(source, rule.type);

          auto right = child_parse(source);
          if (! right) {
            throw RightHandSideExpressionMissingException();
          }

          left = rule.builder(move(left), move(right));

          found = true;
          break;
        }
      }

      if (! found) break;
    }

    return left;
  }

private:
  ParseFn child_parse;
  std::vector<BinaryRule<T>> rules;
};

std::unique_ptr<XPathExpression>
parse_multiplicative_expression(XPathTokenSource &source)
{
  std::vector<BinaryRule<ExpressionMath>> rules = {
    { XPathTokenType::Multiply,  ExpressionMath::Multiplication },
    { XPathTokenType::Divide,    ExpressionMath::Division },
    { XPathTokenType::Remainder, ExpressionMath::Remainder }
  };

  LeftAssociativeBinaryParser<ExpressionMath> parser(parse_unary_expression, rules);
  return parser.parse(source);
}

std::unique_ptr<XPathExpression>
parse_additive_expression(XPathTokenSource &source)
{
  std::vector<BinaryRule<ExpressionMath>> rules = {
    { XPathTokenType::PlusSign,  ExpressionMath::Addition },
    { XPathTokenType::MinusSign, ExpressionMath::Subtraction}
  };

  LeftAssociativeBinaryParser<ExpressionMath> parser(parse_multiplicative_expression, rules);
  return parser.parse(source);
}

std::unique_ptr<XPathExpression>
parse_relational_expression(XPathTokenSource &source)
{
  std::vector<BinaryRule<ExpressionRelational>> rules = {
    { XPathTokenType::LessThan,           ExpressionRelational::LessThan },
    { XPathTokenType::LessThanOrEqual,    ExpressionRelational::LessThanOrEqual },
    { XPathTokenType::GreaterThan,        ExpressionRelational::GreaterThan },
    { XPathTokenType::GreaterThanOrEqual, ExpressionRelational::GreaterThanOrEqual },
  };

  LeftAssociativeBinaryParser<ExpressionRelational> parser(parse_additive_expression, rules);
  return parser.parse(source);
}

std::unique_ptr<XPathExpression>
parse_equality_expression(XPathTokenSource &source)
{
  std::vector<BinaryRule<ExpressionEqual>> rules = {
    { XPathTokenType::Equal,    ExpressionEqual::Equal },
    { XPathTokenType::NotEqual, ExpressionEqual::NotEqual },
  };

  LeftAssociativeBinaryParser<ExpressionEqual> parser(parse_relational_expression, rules);
  return parser.parse(source);
}

std::unique_ptr<XPathExpression>
parse_and_expression(XPathTokenSource &source)
{
  std::vector<BinaryRule<ExpressionAnd>> rules = {
    { XPathTokenType::And, ExpressionAnd::And }
  };

  LeftAssociativeBinaryParser<ExpressionAnd> parser(parse_equality_expression, rules);
  return parser.parse(source);
}

std::unique_ptr<XPathExpression>
parse_or_expression(XPathTokenSource &source)
{
  std::vector<BinaryRule<ExpressionOr>> rules = {
    { XPathTokenType::Or, ExpressionOr::Or }
  };

  LeftAssociativeBinaryParser<ExpressionOr> parser(parse_and_expression, rules);
  return parser.parse(source);
}

std::unique_ptr<XPathExpression>
XPathParser::parse() {
  auto expr = parse_or_expression(_source);

  if (_source.has_more_tokens()) {
    throw ExtraUnparsedTokensException();
  }

  if (expr) return expr;
  return nullptr;
}
