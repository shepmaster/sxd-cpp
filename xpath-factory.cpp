#include "xpath-factory.h"

#include "xpath-tokenizer-buffer.h"
#include "xpath-token-deabbreviator.h"
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
  XPathTokenizer base_tokenizer(xpath);
  XPathTokenDeabbreviator deabbrev(base_tokenizer);
  XPathTokenizerBuffer tokenizer(deabbrev);
  FatalParseErrors fatal_errors;

  XPathParser parser(tokenizer, fatal_errors);
  return XPath(parser.parse());
}
