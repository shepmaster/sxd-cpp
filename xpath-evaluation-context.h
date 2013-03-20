#ifndef XPATH_EVALUATION_CONTEXT_H
#define XPATH_EVALUATION_CONTEXT_H

class XPathFunction;

#include "xpath-function-library.h"
#include "xpath-variable-bindings.h"

class XPathEvaluationContext
{
public:
  virtual ~XPathEvaluationContext() {};
  virtual Node *node() const = 0;
  virtual unsigned long position() const = 0;
  virtual unsigned long size() const = 0;
  virtual void next() = 0;

  virtual bool has_function(std::string name) const = 0;

  virtual std::shared_ptr<XPathFunction>
  function_for_name(std::string name) const = 0;

  virtual bool has_variable(std::string name) const = 0;

  virtual XPathValue
  variable_for_name(std::string name) const = 0;

  virtual std::shared_ptr<XPathEvaluationContext>
  new_context_for(Node *node, unsigned long size) const = 0;
};

#endif
