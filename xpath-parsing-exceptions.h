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

struct MismatchedQuoteCharacterException : virtual std::runtime_error
{
  MismatchedQuoteCharacterException(char actual) :
std::runtime_error(std::string("Unable to find a matching quote character for (") + actual + ")")
  {};
};

#endif
