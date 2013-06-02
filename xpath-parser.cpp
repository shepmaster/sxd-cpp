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
#include "expression-step.h"
#include "expression-predicate.h"
#include "expression-context-node.h"
#include "expression-root-node.h"
#include "expression-path.h"
#include "expression-math.h"
#include "expression-negation.h"
#include "expression-union.h"
#include "expression-or.h"
#include "expression-and.h"
#include "expression-equal.h"
#include "expression-relational.h"
#include "make-unique.h"
#include "xpath-parsing-exceptions.h"

using std::move;

XPathParser::XPathParser(XPathTokenSource &source) :
  _source(XPathParserTokenSource(source))
{
}

using ParseFn = std::unique_ptr<XPathExpression>(*)(XPathParserTokenSource &source);

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
  parse(XPathParserTokenSource &source) {
    auto left = child_parse(source);
    if (! left) return nullptr;

    while (source.has_more_tokens()) {
      bool found = false;

      for (auto rule : rules) {
        if (source.next_token_is(rule.type)) {
          source.consume(rule.type);

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
parse_children_in_order(std::vector<ParseFn> child_parses, XPathParserTokenSource &source)
{
  for (auto child_parse : child_parses) {
    auto expr = child_parse(source);
    if (expr) return expr;
  }

  return nullptr;
}

std::unique_ptr<XPathAxis>
parse_axis(XPathParserTokenSource &source) {
  if (source.next_token_is(XPathTokenType::Axis)) {
    auto token = source.next_token();
    auto name = token.string();
    source.consume(XPathTokenType::DoubleColon);

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
parse_node_test(XPathParserTokenSource &source) {
  if (source.next_token_is(XPathTokenType::NodeTest)) {
    auto token = source.next_token();
    auto name = token.string();

    source.consume(XPathTokenType::LeftParen);
    source.consume(XPathTokenType::RightParen);

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
default_node_test(XPathParserTokenSource &source, std::unique_ptr<XPathAxis> &axis) {
  if (source.next_token_is(XPathTokenType::String)) {
    auto token = source.next_token();

    switch (axis->principal_node_type()) {
    case PrincipalNodeType::Attribute:
      return make_unique<NodeTestAttribute>(token.string());
    case PrincipalNodeType::Element:
      return make_unique<NodeTestElement>(token.prefixed_name());
    }
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_variable_reference(XPathParserTokenSource &source)
{
  if (source.next_token_is(XPathTokenType::DollarSign)) {
    source.consume(XPathTokenType::DollarSign);
    auto token = source.consume(XPathTokenType::String);

    return make_unique<ExpressionVariable>(token.string());
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_string_literal(XPathParserTokenSource &source) {
  if (source.next_token_is(XPathTokenType::Literal)) {
    auto token = source.next_token();
    return make_unique<ExpressionLiteral>(token.string());
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_numeric_literal(XPathParserTokenSource &source)
{
  if (source.next_token_is(XPathTokenType::Number)) {
    auto token = source.next_token();
    return make_unique<ExpressionLiteral>(token.number());
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_primary_expression(XPathParserTokenSource &source);

std::unique_ptr<XPathExpression>
parse_function_call(XPathParserTokenSource &source)
{
  if (source.next_token_is(XPathTokenType::Function)) {
    auto token = source.next_token();

    std::vector<std::shared_ptr<XPathExpression>> arguments;

    source.consume(XPathTokenType::LeftParen);
    while (! source.next_token_is(XPathTokenType::RightParen)) {
      // TODO: this should be the top-level expression
      auto arg = parse_primary_expression(source);
      if (! arg) break;
      arguments.push_back(move(arg));
    }
    source.consume(XPathTokenType::RightParen);

    return make_unique<ExpressionFunction>(token.string(), std::move(arguments));
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_primary_expression(XPathParserTokenSource &source) {
  std::vector<ParseFn> child_parses = {
    parse_variable_reference,
    parse_string_literal,
    parse_numeric_literal,
    parse_function_call
  };

  return parse_children_in_order(child_parses, source);
}

std::unique_ptr<XPathExpression>
parse_predicate_expression(XPathParserTokenSource &source)
{
  if (source.next_token_is(XPathTokenType::LeftBracket)) {
    source.consume(XPathTokenType::LeftBracket);

    // TODO: This should be the top-level expression
    auto predicate = parse_primary_expression(source);
    if (! predicate) {
      throw EmptyPredicateException();
    }

    source.consume(XPathTokenType::RightBracket);

    return predicate;
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_step(XPathParserTokenSource &source)
{
  auto axis = parse_axis(source);

  auto node_test = parse_node_test(source);
  if (! node_test) {
    node_test = default_node_test(source, axis);
  }
  if (! node_test) {
    return nullptr;
  }

  return make_unique<ExpressionStep>(move(axis), move(node_test));
}

std::unique_ptr<XPathExpression>
parse_predicates(XPathParserTokenSource &source,
                 std::unique_ptr<XPathExpression> node_selecting_expr)
{
  while (auto predicate_expr = parse_predicate_expression(source)) {
    node_selecting_expr = make_unique<ExpressionPredicate>(move(node_selecting_expr),
                                                           move(predicate_expr));
  }

  return node_selecting_expr;
}

std::unique_ptr<XPathExpression>
parse_relative_location_path_raw(XPathParserTokenSource &source,
                                 std::unique_ptr<XPathExpression> start_point)
{
  std::vector<std::unique_ptr<XPathExpression>> steps;

  auto step = parse_step(source);
  if (step) {
    step = parse_predicates(source, move(step));
    steps.push_back(move(step));

    while (source.next_token_is(XPathTokenType::Slash)) {
      source.consume(XPathTokenType::Slash);

      auto next = parse_step(source);
      if (! next) {
        throw TrailingSlashException();
      }

      next = parse_predicates(source, move(next));
      steps.push_back(move(next));
    }

    return make_unique<ExpressionPath>(move(start_point), move(steps));
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_relative_location_path(XPathParserTokenSource &source)
{
  auto start_point = make_unique<ExpressionContextNode>();
  return parse_relative_location_path_raw(source, move(start_point));
}

std::unique_ptr<XPathExpression>
parse_absolute_location_path(XPathParserTokenSource &source)
{
  if (source.next_token_is(XPathTokenType::Slash)) {
    source.consume(XPathTokenType::Slash);

    auto start_point = make_unique<ExpressionRootNode>();
    auto expr = parse_relative_location_path_raw(source, move(start_point));
    if (expr) return expr;

    return make_unique<ExpressionRootNode>();
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_location_path(XPathParserTokenSource &source)
{
  std::vector<ParseFn> child_parses = {
    parse_relative_location_path,
    parse_absolute_location_path
  };

  return parse_children_in_order(child_parses, source);
}

std::unique_ptr<XPathExpression>
parse_filter_expression(XPathParserTokenSource &source)
{
  auto expr = parse_primary_expression(source);
  if (expr) {
    return parse_predicates(source, move(expr));
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_path_expression(XPathParserTokenSource &source)
{
  auto expr = parse_location_path(source);
  if (expr) return expr;

  auto filter = parse_filter_expression(source);
  if (filter) {
    if (source.next_token_is(XPathTokenType::Slash)) {
      source.consume(XPathTokenType::Slash);

      filter = parse_relative_location_path_raw(source, move(filter));
      if (! filter) {
        throw TrailingSlashException();
      }

      return filter;
    }

    return filter;
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_union_expression(XPathParserTokenSource &source)
{
  std::vector<BinaryRule<ExpressionUnion>> rules = {
    { XPathTokenType::Pipe, ExpressionUnion::Union }
  };

  LeftAssociativeBinaryParser<ExpressionUnion> parser(parse_path_expression, rules);
  return parser.parse(source);
}

std::unique_ptr<XPathExpression>
parse_unary_expression(XPathParserTokenSource &source)
{
  auto expr = parse_union_expression(source);
  if (expr) return expr;

  if (source.next_token_is(XPathTokenType::MinusSign)) {
    source.consume(XPathTokenType::MinusSign);

    expr = parse_unary_expression(source);
    if (! expr) {
      throw RightHandSideExpressionMissingException();
    }

    return make_unique<ExpressionNegation>(move(expr));
  }

  return nullptr;
}

std::unique_ptr<XPathExpression>
parse_multiplicative_expression(XPathParserTokenSource &source)
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
parse_additive_expression(XPathParserTokenSource &source)
{
  std::vector<BinaryRule<ExpressionMath>> rules = {
    { XPathTokenType::PlusSign,  ExpressionMath::Addition },
    { XPathTokenType::MinusSign, ExpressionMath::Subtraction}
  };

  LeftAssociativeBinaryParser<ExpressionMath> parser(parse_multiplicative_expression, rules);
  return parser.parse(source);
}

std::unique_ptr<XPathExpression>
parse_relational_expression(XPathParserTokenSource &source)
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
parse_equality_expression(XPathParserTokenSource &source)
{
  std::vector<BinaryRule<ExpressionEqual>> rules = {
    { XPathTokenType::Equal,    ExpressionEqual::Equal },
    { XPathTokenType::NotEqual, ExpressionEqual::NotEqual },
  };

  LeftAssociativeBinaryParser<ExpressionEqual> parser(parse_relational_expression, rules);
  return parser.parse(source);
}

std::unique_ptr<XPathExpression>
parse_and_expression(XPathParserTokenSource &source)
{
  std::vector<BinaryRule<ExpressionAnd>> rules = {
    { XPathTokenType::And, ExpressionAnd::And }
  };

  LeftAssociativeBinaryParser<ExpressionAnd> parser(parse_equality_expression, rules);
  return parser.parse(source);
}

std::unique_ptr<XPathExpression>
parse_or_expression(XPathParserTokenSource &source)
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
