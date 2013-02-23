#include "xpath-factory.h"

#include "xpath-tokenizer-buffer.h"
#include "xpath-parser.h"

#include <memory>

class FatalParseErrors : public XPathParseErrorNotifier
{
public:
  void invalid_axis(std::string axis_name) {
    throw InvalidXPathAxisException(axis_name);
  }

  void invalid_node_test(std::string name) {
    throw InvalidNodeTestException(name);
  }
};

XPath
XPathFactory::compile(std::string xpath)
{
  auto base_tokenizer = XPathTokenizer(xpath);
  auto tokenizer = XPathTokenizerBuffer(base_tokenizer);
  FatalParseErrors fatal_errors;

  XPathParser parser(tokenizer, fatal_errors);
  return XPath(parser.parse());
}
