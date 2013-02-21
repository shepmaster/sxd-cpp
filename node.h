#ifndef NODE_H
#define NODE_H

class Document;
class Nodeset;
class Node;

#include <functional>
#include "output.h"
#include "to-stream.h"

#include <vector>

typedef enum {
  NODE_TYPE_ELEMENT,
  NODE_TYPE_ATTRIBUTE,
  NODE_TYPE_TEXT_NODE,
  NODE_TYPE_COMMENT
} node_type_t;

class NodeSelector {
public:
  virtual Nodeset select_nodes(Node *node) = 0;
};

class Node : public ToStream {
public:
  typedef std::function<void (Node *)> foreach_fn_t;

  Node(Document *document, node_type_t type);
  virtual ~Node() {};
  virtual void change_document(Document *doc);
  virtual void output(Output &output) const = 0;
  void output_children(Output &output) const;

  void append_child(Node *child);
  void remove_child(Node *child);

  Node *first_child() const;
  Node *parent() const;
  Node *prev_sibling() const;
  Node *next_sibling() const;

  Document *document();

  void foreach_child(foreach_fn_t fn) const;
  void foreach_preceding_sibling(foreach_fn_t fn) const;
  void foreach_following_sibling(foreach_fn_t fn) const;
  void foreach_ancestor(foreach_fn_t fn) const;

  node_type_t type();

  Nodeset select_nodes(NodeSelector &selector);

protected:
  node_type_t _type;
  Document *_doc;
  Node *_parent;
  std::vector<Node *>_children;
};

#endif
