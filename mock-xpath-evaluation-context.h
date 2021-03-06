#ifndef MOCK_XPATH_EVALUATION_CONTEXT_H
#define MOCK_XPATH_EVALUATION_CONTEXT_H

#include "xpath-evaluation-context.h"

class MockEvaluationContext : public XPathEvaluationContext {
public:
  MOCK_CONST_METHOD0(node, Node *());
  MOCK_CONST_METHOD0(position, unsigned long());
  MOCK_CONST_METHOD0(size, unsigned long());
  MOCK_METHOD1(next, void(Node *context_node));
  MOCK_CONST_METHOD1(has_function, bool(std::string name));
  MOCK_CONST_METHOD1(function_for_name, std::shared_ptr<XPathFunction>(std::string name));
  MOCK_CONST_METHOD1(has_variable, bool(std::string name));
  MOCK_CONST_METHOD1(variable_for_name, XPathValue(std::string name));
  MOCK_CONST_METHOD1(find_namespace_for_prefix, std::string *(std::string prefix));
  MOCK_CONST_METHOD1(new_context_for,
                       std::shared_ptr<XPathEvaluationContext>(unsigned long size));
};

#endif
