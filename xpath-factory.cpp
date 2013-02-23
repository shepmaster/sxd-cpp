#include "xpath-factory.h"

#include "xpath-tokenizer-buffer.h"
#include "xpath-token-deabbreviator.h"
#include "xpath-parser.h"

#include <memory>

XPath
XPathFactory::compile(std::string xpath)
{
  XPathTokenizer base_tokenizer(xpath);
  XPathTokenDeabbreviator deabbrev(base_tokenizer);
  XPathTokenizerBuffer tokenizer(deabbrev);

  XPathParser parser(tokenizer);
  return XPath(parser.parse());
}
