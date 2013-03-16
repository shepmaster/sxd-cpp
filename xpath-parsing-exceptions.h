#ifndef XPATH_PARSING_EXCEPTIONS_H
#define XPATH_PARSING_EXCEPTIONS_H

#include <stdexcept>

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

#endif
