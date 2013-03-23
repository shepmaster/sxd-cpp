#include "xpath-factory.h"

#include "xpath-parser-token-source.h"
#include "xpath-token-deabbreviator.h"
#include "xpath-token-disambiguator.h"
#include "xpath-parser.h"

#include <memory>

XPath
XPathFactory::compile(std::string xpath)
{
  XPathTokenizer base_tokenizer(xpath);
  XPathTokenDeabbreviator deabbreviator(base_tokenizer);
  XPathTokenDisambiguator disambiguator(deabbreviator);

  XPathParser parser(disambiguator);
  return XPath(parser.parse());
}
