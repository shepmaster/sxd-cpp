#ifndef XPATH_INTERNAL_H
#define XPATH_INTERNAL_H

#include <vector>
#include <glib.h>
#include "xpath.h"
#include "node.h"
#include "nodeset.h"

typedef enum {
  SLASH,
  LBRACKET,
  RBRACKET,
  LPAREN,
  RPAREN,
  QUOTE,
  APOS,
  TEXT
} xpath_token_type_t;

typedef struct {
  xpath_token_type_t type;
  unsigned int start;
} xpath_token_t;

typedef struct {
  char *xpath;
  std::vector<xpath_token_t> tokens;
} xpath_tokens_t;

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

#define XPATH_NODE_TYPE_ELEMENT (1 << 0)
#define XPATH_NODE_TYPE_TEXT_NODE (1 << 1)
typedef unsigned char xpath_node_type_t;

typedef struct xpath_resultS xpath_result_t;
typedef struct xpath_evaluation_contextS xpath_evaluation_context_t;

class XPathPredicate {
public:
  virtual xpath_result_t eval(xpath_evaluation_context_t *context) = 0;
};

typedef struct {
  xpath_axis_t axis;
  xpath_node_type_t type;
  char *name;
  std::vector<XPathPredicate *> predicates;
} xpath_step_t;

typedef struct {
  std::vector<xpath_step_t> steps;
} xpath_compiled_t;

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

void
xpath_tokens_free(xpath_tokens_t *tokens);

xpath_tokens_t *
xpath_tokenize(const char * const xpath);

char *
xpath_tokens_string(xpath_tokens_t *tokens, int index);

void
xpath_compiled_free(xpath_compiled_t *compiled);

xpath_compiled_t *
xpath_compile(const char * const xpath);

Nodeset *
xpath_select_xpath_no_predicates(Node *node, xpath_step_t *step);

Nodeset *
xpath_apply_predicates(Nodeset *nodeset, xpath_step_t *step);

Nodeset *
xpath_select_xpath_steps(Node *node, std::vector<xpath_step_t> &steps);

Nodeset *
xpath_apply_xpath(Node *node, const char * const xpath);

void
xpath_result_destroy(xpath_result_t *result);

#endif
