#ifndef NODE_H
#define NODE_H

typedef class Node node_t;
typedef enum {
  NODE_TYPE_ELEMENT,
  NODE_TYPE_TEXT_NODE,
  NODE_TYPE_COMMENT
} node_type_t;

typedef struct documentS document_t;

#include "nodeset.h"

typedef void (*node_foreach_fn_t)(node_t *node, gpointer user_data);

#include "output.h"

class Node {
public:
  Node(document_t *document, node_type_t type);
  virtual ~Node();
  virtual void change_document(document_t *doc);
  virtual void output(output_t *output) = 0;
  void output_children(output_t *output);

  void append_child(Node *child);
  void remove_child(Node *child);

  node_t *first_child();
  node_t *parent();
  node_t *prev_sibling();
  node_t *next_sibling();
  void insert_next_sibling(node_t *new_sibling);

  document_t *document();

  void foreach_child(node_foreach_fn_t fn, gpointer user_data);
  void foreach_preceding_sibling(node_foreach_fn_t fn, gpointer user_data);
  void foreach_following_sibling(node_foreach_fn_t fn, gpointer user_data);
  void foreach_ancestor(node_foreach_fn_t fn, gpointer user_data);

  node_type_t type();

protected:
  node_type_t type_;
  document_t *doc;
  node_t *parent_;
  node_t *first_child_;
  node_t *prev_sibling_;
  node_t *next_sibling_;
};

#endif
