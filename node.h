#ifndef NODE_H
#define NODE_H

class Document;
class Node;

#include <glib.h>
#include "output.h"

typedef enum {
  NODE_TYPE_ELEMENT,
  NODE_TYPE_TEXT_NODE,
  NODE_TYPE_COMMENT
} node_type_t;

typedef void (*node_foreach_fn_t)(Node *node, gpointer user_data);

class Node {
public:
  Node(Document *document, node_type_t type);
  virtual ~Node();
  virtual void change_document(Document *doc);
  virtual void output(Output &output) = 0;
  void output_children(Output &output);

  void append_child(Node *child);
  void remove_child(Node *child);

  Node *first_child();
  Node *parent();
  Node *prev_sibling();
  Node *next_sibling();
  void insert_next_sibling(Node *new_sibling);

  Document *document();

  void foreach_child(node_foreach_fn_t fn, gpointer user_data);
  void foreach_preceding_sibling(node_foreach_fn_t fn, gpointer user_data);
  void foreach_following_sibling(node_foreach_fn_t fn, gpointer user_data);
  void foreach_ancestor(node_foreach_fn_t fn, gpointer user_data);

  node_type_t type();

protected:
  node_type_t type_;
  Document *doc;
  Node *parent_;
  Node *first_child_;
  Node *prev_sibling_;
  Node *next_sibling_;
};

#endif
