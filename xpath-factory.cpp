#include "xpath-factory.h"

#include "xpath-tokenizer-buffer.h"
#include "xpath-parser.h"

class XPathStepCapture : public XPathCreator
{
public:
  void add_step(std::unique_ptr<XPathStep> step) {
    steps.push_back(std::move(step));
  }

  void invalid_axis(std::string axis_name) {
    throw InvalidXPathAxisException(axis_name);
  }

  XPath finalize() {
    return XPath(std::move(steps));
  }

private:
  std::vector<std::unique_ptr<XPathStep>> steps;
};

XPath
XPathFactory::compile(std::string xpath)
{
  auto tokenizer = XPathTokenizerBuffer(XPathTokenizer(xpath));
  XPathStepCapture captor;

  XPathParser parser(tokenizer, captor);
  parser.parse();

  return captor.finalize();
}
