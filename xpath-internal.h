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

class XPathStep;

class StepTester {
public:
  StepTester(XPathStep &step);

  void operator() (Node *node);
  Nodeset selected_nodes();

private:
  XPathStep &_step;
  Nodeset _nodeset;
};

class XPathAxis {
public:
  virtual ~XPathAxis() {};
  virtual void traverse(Node *node, StepTester &test) = 0;
};

class AxisSelf : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

class AxisChild : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

class AxisParent : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

class AxisFollowingSibling : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

class AxisPrecedingSibling : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

class AxisDescendant : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

class AxisDescendantOrSelf : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

class AxisAncestor : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

class AxisAncestorOrSelf : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

class AxisFollowing : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

class AxisPreceding : public XPathAxis {
public:
  void traverse(Node *node, StepTester &test);
};

class XPathStep {
public:
  XPathStep(XPathAxis *axis);
  XPathAxis *axis;
  std::vector<XPathNodeTest *> tests;
  std::vector<XPathPredicate *> predicates;
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

Nodeset
xpath_select_xpath_no_predicates(Node *node, XPathStep &step);

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
