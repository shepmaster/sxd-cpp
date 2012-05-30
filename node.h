#ifndef NODE_H
#define NODE_H

class Node;

#include <glib.h>
#include "nodeset.h"
#include "output.h"

typedef enum {
  NODE_TYPE_ELEMENT,
  NODE_TYPE_TEXT_NODE,
  NODE_TYPE_COMMENT
} node_type_t;

typedef struct documentS document_t;

typedef void (*node_foreach_fn_t)(Node *node, gpointer user_data);

class Node {
public:
  Node(document_t *document, node_type_t type);
  virtual ~Node();
  virtual void change_document(document_t *doc);
  virtual void output(output_t *output) = 0;
  void output_children(output_t *output);

  void append_child(Node *child);
  void remove_child(Node *child);

  Node *first_child();
  Node *parent();
  Node *prev_sibling();
  Node *next_sibling();
  void insert_next_sibling(Node *new_sibling);

  document_t *document();

  void foreach_child(node_foreach_fn_t fn, gpointer user_data);
  void foreach_preceding_sibling(node_foreach_fn_t fn, gpointer user_data);
  void foreach_following_sibling(node_foreach_fn_t fn, gpointer user_data);
  void foreach_ancestor(node_foreach_fn_t fn, gpointer user_data);

  node_type_t type();

protected:
  node_type_t type_;
  document_t *doc;
  Node *parent_;
  Node *first_child_;
  Node *prev_sibling_;
  Node *next_sibling_;
};

#endif
