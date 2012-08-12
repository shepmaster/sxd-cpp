#ifndef XPATH_INTERNAL_H
#define XPATH_INTERNAL_H

#include <vector>
#include <glib.h>
#include "xpath.h"
#include "node.h"
#include "nodeset.h"

typedef struct xpath_resultS xpath_result_t;
typedef struct xpath_evaluation_contextS xpath_evaluation_context_t;

class XPathPredicate {
public:
  virtual xpath_result_t eval(xpath_evaluation_context_t *context) = 0;
};

#include "xpath-node-test.h"
#include "xpath-axis.h"

class XPathStep {
public:
  XPathStep(XPathAxis *axis);

  Nodeset select_without_predicates(Node *node);
  Nodeset select_with_predicates(Nodeset node);

  std::vector<XPathNodeTest *> tests;
private:
  XPathAxis *axis;
  std::vector<XPathPredicate *> _predicates;
};

typedef enum {
  XPATH_RESULT_TYPE_BOOLEAN,
  XPATH_RESULT_TYPE_NUMERIC,
  XPATH_RESULT_TYPE_STRING,
  XPATH_RESULT_TYPE_NODESET
} xpath_result_type_t;

typedef struct xpath_resultS {
  xpath_result_type_t type;

  union {
    unsigned int boolean:1;
    double numeric;
    char *string;
    Nodeset *nodeset;
  } value;
} xpath_result_t;

typedef struct xpath_evaluation_contextS {
  Node *node;
  Nodeset *nodeset;
} xpath_evaluation_context_t;

typedef std::vector<xpath_result_t> xpath_parameters_t;

typedef xpath_result_t (*xpath_fn_t)(xpath_evaluation_context_t *context, xpath_parameters_t &parameters);

class XPathProcessor {
public:
  XPathProcessor(Node *node);

  Nodeset apply(const char * const xpath);
  Nodeset select_steps(std::vector<XPathStep> &steps);

private:
  Node *_node;
};

class PotentialNodes {
public:
  void add_candidates(Nodeset &nodeset);
  Nodeset apply_predicates(std::vector<XPathPredicate *> predicates);

private:
  Nodeset _nodeset;
};

void
xpath_result_destroy(xpath_result_t *result);

#endif
