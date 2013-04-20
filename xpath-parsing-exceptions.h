#ifndef XPATH_PARSING_EXCEPTIONS_H
#define XPATH_PARSING_EXCEPTIONS_H

#include <stdexcept>

struct ExtraUnparsedTokensException : virtual std::runtime_error
{
 ExtraUnparsedTokensException() :
    std::runtime_error("Unparsed XPath tokens remain.")
  {};
};

struct UnexpectedTokenException : virtual std::runtime_error
{
 UnexpectedTokenException() :
    std::runtime_error("Encountered an unexpected token.")
  {};
};

struct RightHandSideExpressionMissingException : virtual std::runtime_error
{
 RightHandSideExpressionMissingException() :
    std::runtime_error("The right-hand side of an operator is missing.")
  {};
};

struct EmptyPredicateException : virtual std::runtime_error
{
 EmptyPredicateException() :
    std::runtime_error("Predicate expressions must not be empty.")
  {};
};

struct TrailingSlashException : virtual std::runtime_error
{
 TrailingSlashException() :
    std::runtime_error("Path expressions must not end with a slash.")
  {};
};

struct InvalidXPathAxisException : virtual std::runtime_error
{
  InvalidXPathAxisException(std::string axis) :
    std::runtime_error(axis)
  {};
};

struct UnknownXPathFunctionException : virtual std::runtime_error
{
  UnknownXPathFunctionException(std::string name) :
    std::runtime_error(name)
  {};
};

struct UnknownVariableException : virtual std::runtime_error
{
  UnknownVariableException(std::string name) :
    std::runtime_error(name)
  {};
};

struct InvalidNodeTestException : virtual std::runtime_error
{
  InvalidNodeTestException(std::string name) :
    std::runtime_error(name)
  {};
};

struct NoMoreTokensAvailableException : virtual std::runtime_error
{
  NoMoreTokensAvailableException() :
    std::runtime_error("No more tokens are available")
  {};
};

struct UnableToCreateTokenException : virtual std::runtime_error
{
  UnableToCreateTokenException() :
    std::runtime_error("Unable to tokenize further")
  {};
};

struct MissingLocalNameException : virtual std::runtime_error
{
  MissingLocalNameException() :
    std::runtime_error("A namespace prefix must be followed by a local name")
  {};
};

struct MismatchedQuoteCharacterException : virtual std::runtime_error
{
  MismatchedQuoteCharacterException(char actual) :
std::runtime_error(std::string("Unable to find a matching quote character for (") + actual + ")")
  {};
};

#endif
