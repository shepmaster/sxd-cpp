#include <iostream>

#include "document.h"
#include "nodeset.h"

void
Node::foreach_child(foreach_fn_t fn) const
{
  Node *child = _first_child;
  while (child) {
    Node *next = child->_next_sibling;
    fn(child);
    child = next;
  }
}

void
Node::foreach_preceding_sibling(foreach_fn_t fn) const
{
  Node *sibling = _prev_sibling;
  while (sibling) {
    Node *next = sibling->_prev_sibling;
    fn(sibling);
    sibling = next;
  }
}

void
Node::foreach_following_sibling(foreach_fn_t fn) const
{
  Node *sibling = _next_sibling;
  while (sibling) {
    Node *next = sibling->_next_sibling;
    fn(sibling);
    sibling = next;
  }
}

void
Node::foreach_ancestor(foreach_fn_t fn) const
{
  Node *parent;
  for (parent = _parent; parent; parent = parent->_parent) {
    fn(parent);
  }
}

struct FreeChildren {
  virtual void operator() (Node *node) { delete node; }
};

Node::~Node()
{
  if (_parent) {
    _parent->remove_child(this);
  }

  foreach_child(FreeChildren());
}

Node::Node(Document *doc, node_type_t type) :
  _type(type), _doc(doc),
  _parent(nullptr),
  _first_child(nullptr),
  _prev_sibling(nullptr),
  _next_sibling(nullptr)
{
}

node_type_t
Node::type()
{
  return _type;
}

Node *
node_cast_to_node(Node *n)
{
  return n;
}

void
Node::append_child(Node *child)
{
  Node *my_child = _first_child;
  if (my_child) {
    while (my_child->_next_sibling) {
      my_child = my_child->_next_sibling;
    }
    my_child->insert_next_sibling(child);
  } else {
    _first_child = child;
  }
  child->_parent = this;
}

void
Node::remove_child(Node *child)
{
  if (child->_prev_sibling) {
    child->_prev_sibling->_next_sibling = child->_next_sibling;
  }

  if (child->_next_sibling) {
    child->_next_sibling->_prev_sibling = child->_prev_sibling;
  }

  if (child->_parent && child->_parent->_first_child == child) {
    child->_parent->_first_child = child->_next_sibling;
  }
  child->_parent = nullptr;
}

Node *
Node::first_child() const
{
  return _first_child;
}

Node *
Node::parent() const
{
  return _parent;
}

Node *
Node::prev_sibling() const
{
  return _prev_sibling;
}

Node *
Node::next_sibling() const
{
  return _next_sibling;
}

void
Node::insert_next_sibling(Node *new_sibling)
{
  if (_next_sibling) {
    _next_sibling->_prev_sibling = new_sibling;
  }

  new_sibling->_prev_sibling = this;
  new_sibling->_next_sibling = _next_sibling;

  _next_sibling = new_sibling;
}

Document *
Node::document()
{
  return _doc;
}

struct ChangeDocumentChildren {
  ChangeDocumentChildren(Document *doc) : _doc(doc) {}
  void operator()(Node *node) {  node->change_document(_doc); }
  Document *_doc;
};

void
Node::change_document(Document *doc)
{
  _doc = doc;

  foreach_child(ChangeDocumentChildren(doc));
}

struct OutputWrapper {
  OutputWrapper(Output &output) : output(output) {}
  void operator() (Node *node) { node->output(output); }
  Output &output;
};

void
Node::output_children(Output &output) const
{
  foreach_child(OutputWrapper(output));
}

Nodeset
Node::select_nodes(NodeSelector &selector)
{
  return selector.select_nodes(this);
}
