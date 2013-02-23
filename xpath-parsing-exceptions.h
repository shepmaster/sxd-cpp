#ifndef XPATH_PARSING_EXCEPTIONS_H
#define XPATH_PARSING_EXCEPTIONS_H

#include <stdexcept>

struct InvalidXPathAxisException : virtual std::runtime_error
{
  InvalidXPathAxisException(std::string axis) :
    std::runtime_error(axis)
  {};
};

struct InvalidNodeTestException : virtual std::runtime_error
{
  InvalidNodeTestException(std::string name) :
    std::runtime_error(name)
  {};
};

#endif
