#ifndef NODE_H
#define NODE_H

class Document;
class Nodeset;
class Node;

#include <functional>
#include "output.h"

typedef enum {
  NODE_TYPE_ELEMENT,
  NODE_TYPE_TEXT_NODE,
  NODE_TYPE_COMMENT
} node_type_t;

class NodeSelector {
public:
  virtual Nodeset select_nodes(Node &node) = 0;
};

class Node {
public:
  typedef std::function<void (Node *)> foreach_fn_t;

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

  void foreach_child(foreach_fn_t fn);
  void foreach_preceding_sibling(foreach_fn_t fn);
  void foreach_following_sibling(foreach_fn_t fn);
  void foreach_ancestor(foreach_fn_t fn);

  node_type_t type();

  Nodeset select_nodes(NodeSelector &selector);

protected:
  node_type_t type_;
  Document *doc;
  Node *parent_;
  Node *first_child_;
  Node *prev_sibling_;
  Node *next_sibling_;
};

#endif
