#ifndef XPATH_INTERNAL_H
#define XPATH_INTERNAL_H

#include <vector>
#include <glib.h>
#include "xpath.h"
#include "node.h"
#include "nodeset.h"

typedef enum {
  XPATH_AXIS_ANCESTOR,
  XPATH_AXIS_ANCESTOR_OR_SELF,
  XPATH_AXIS_ATTRIBUTE,
  XPATH_AXIS_CHILD,
  XPATH_AXIS_DESCENDANT,
  XPATH_AXIS_DESCENDANT_OR_SELF,
  XPATH_AXIS_FOLLOWING,
  XPATH_AXIS_FOLLOWING_SIBLING,
  XPATH_AXIS_NAMESPACE,
  XPATH_AXIS_PARENT,
  XPATH_AXIS_PRECEDING,
  XPATH_AXIS_PRECEDING_SIBLING,
  XPATH_AXIS_SELF
} xpath_axis_t;

typedef struct xpath_resultS xpath_result_t;
typedef struct xpath_evaluation_contextS xpath_evaluation_context_t;

class XPathPredicate {
public:
  virtual xpath_result_t eval(xpath_evaluation_context_t *context) = 0;
};

class XPathNodeTest {
public:
  virtual ~XPathNodeTest() {};
  virtual bool include_node(Node &node) = 0;
};

#include <string>

class ElementTest : public XPathNodeTest {
public:
  bool include_node(Node &node);
};

class NamedElementTest : public XPathNodeTest {
public:
  NamedElementTest(std::string name);
  bool include_node(Node &node);

private:
  std::string _name;
};

class TextTest : public XPathNodeTest {
public:
  bool include_node(Node &node);
};

typedef struct {
  xpath_axis_t axis;
  std::vector<XPathNodeTest *> tests;
  std::vector<XPathPredicate *> predicates;
} xpath_step_t;

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

Nodeset
xpath_select_xpath_no_predicates(Node *node, xpath_step_t *step);

class XPathProcessor {
public:
  XPathProcessor(Node *node);

  Nodeset apply(const char * const xpath);
  Nodeset select_steps(std::vector<xpath_step_t> &steps);

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
