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

typedef enum {
  XPATH_NODE_TYPE_ELEMENT = 1 << 0,
  XPATH_NODE_TYPE_TEXT_NODE = 1 << 1
} xpath_node_type_t;

typedef struct {
  xpath_axis_t axis;
  xpath_node_type_t type;
  char *name;
} xpath_step_t;

typedef struct {
  GArray *steps;
} xpath_compiled_t;

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
xpath_select_xpath(node_t *node, xpath_step_t *step);

nodeset_t *
xpath_apply_xpath(node_t *node, const char * const xpath);

#endif
