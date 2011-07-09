#ifndef XPATH_INTERNAL_H
#define XPATH_INTERNAL_H

#include <glib.h>
#include "xpath.h"
#include "node.h"

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
  GArray *tokens;
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

typedef struct {
  xpath_axis_t axis;
  xpath_node_type_t type;
  char *name;
  GList *predicates;
} xpath_step_t;

typedef struct {
  GArray *steps;
} xpath_compiled_t;

typedef enum {
  XPATH_RESULT_TYPE_BOOLEAN,
  XPATH_RESULT_TYPE_NUMERIC,
  XPATH_RESULT_TYPE_STRING,
  XPATH_RESULT_TYPE_NODESET
} xpath_result_type_t;

typedef struct {
  xpath_result_type_t type;

  union {
    unsigned int boolean:1;
    double numeric;
    char *string;
    nodeset_t *nodeset;
  } value;
} xpath_result_t;

typedef struct {
  node_t *node;
  nodeset_t *nodeset;
} xpath_evaluation_context_t;

typedef xpath_result_t (*xpath_fn_t)(xpath_evaluation_context_t *context, GArray *parameters);

typedef enum {
  XPATH_PREDICATE_OP_VALUE,
  XPATH_PREDICATE_OP_FUNCTION,
  XPATH_PREDICATE_OP_EQUAL
} xpath_predicate_op_t;

struct xpath_predicateS {
  xpath_predicate_op_t op;
  union {
    xpath_result_t value;
    struct {
      xpath_fn_t fn;
      GArray *parameters;
    } function;
    struct {
      struct xpath_predicateS *left;
      struct xpath_predicateS *right;
    } child;
  } info;
};

typedef struct xpath_predicateS xpath_predicate_t;

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

nodeset_t *
xpath_select_xpath_no_predicates(node_t *node, xpath_step_t *step);

nodeset_t *
xpath_apply_predicates(nodeset_t *nodeset, xpath_step_t *step);

nodeset_t *
xpath_select_xpath_steps(node_t *node, GArray *steps);

nodeset_t *
xpath_apply_xpath(node_t *node, const char * const xpath);

/* XPath 1.0 Core Function Library */
/* 4.1 - Node Set Functions */

xpath_result_t
xpath_fn_last(xpath_evaluation_context_t *context, GArray *parameters_unused);

xpath_result_t
xpath_fn_position(xpath_evaluation_context_t *context, GArray *parameters_unused);

xpath_result_t
xpath_fn_count(xpath_evaluation_context_t *context_unused, GArray *parameters);

/* 4.2 - String Functions */

xpath_result_t
xpath_fn_concat(xpath_evaluation_context_t *context_unused, GArray *parameters);

xpath_result_t
xpath_fn_starts_with(xpath_evaluation_context_t *context_unused, GArray *parameters);

xpath_result_t
xpath_fn_contains(xpath_evaluation_context_t *context_unused, GArray *parameters);

xpath_result_t
xpath_fn_substring_before(xpath_evaluation_context_t *context_unused, GArray *parameters);

xpath_result_t
xpath_fn_substring_after(xpath_evaluation_context_t *context_unused, GArray *parameters);

xpath_result_t
xpath_fn_substring(xpath_evaluation_context_t *context_unused, GArray *parameters);

/* 4.3 - Boolean Functions */

xpath_result_t
xpath_fn_not(xpath_evaluation_context_t *context_unused, GArray *parameters);

xpath_result_t
xpath_fn_true(xpath_evaluation_context_t *context_unused, GArray *parameters_unused);

xpath_result_t
xpath_fn_false(xpath_evaluation_context_t *context_unused, GArray *parameters_unused);

/* 4.4 - Number Functions */

xpath_result_t
xpath_fn_floor(xpath_evaluation_context_t *context_unused, GArray *parameters);

xpath_result_t
xpath_fn_ceiling(xpath_evaluation_context_t *context_unused, GArray *parameters);

xpath_result_t
xpath_fn_round(xpath_evaluation_context_t *context_unused, GArray *parameters);

#endif
