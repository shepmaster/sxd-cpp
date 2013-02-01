#include "xpath-factory.h"

#include "xpath-tokenizer-buffer.h"
#include "xpath-parser.h"

XPath
XPathFactory::compile(std::string xpath)
{
  auto tokenizer = XPathTokenizerBuffer(XPathTokenizer(xpath));

  XPath result;
  auto s = [&](std::vector<std::unique_ptr<XPathStep>> &&parts) {
    result = XPath(std::move(parts));
  };

  XPathParser parser(tokenizer, s);
  parser.parse();

  return result;
}
