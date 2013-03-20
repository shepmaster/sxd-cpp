#ifndef EXPRESSION_PREDICATE_H
#define EXPRESSION_PREDICATE_H

#include "xpath-expression.h"

class ExpressionPredicate : public XPathExpression
{
public:
  typedef std::shared_ptr<XPathExpression> SubExpression;

  ExpressionPredicate(SubExpression node_selecting_expression,
                      SubExpression predicate);

  XPathValue evaluate(const XPathEvaluationContext &context) const;

private:
  SubExpression _node_selecting_expression;
  SubExpression _predicate;
};

#endif
