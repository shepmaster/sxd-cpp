#include <iostream>

#include "document.h"
#include "nodeset.h"

void
Node::foreach_child(foreach_fn_t fn) const
{
  for (auto child : _children) {
    fn(child);
  }
}

void
Node::foreach_preceding_sibling(foreach_fn_t fn) const
{
  auto in_position = std::find(_parent->_children.rbegin(), _parent->_children.rend(), this);
  for (auto sibling = in_position; sibling != _parent->_children.rend(); ++sibling) {
    fn(*sibling);
  }
}

void
Node::foreach_following_sibling(foreach_fn_t fn) const
{
  auto in_position = std::find(_parent->_children.begin(), _parent->_children.end(), this);
  for (auto sibling = in_position; sibling != _parent->_children.end(); ++sibling) {
    fn(*sibling);
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

Node::Node(Document *doc, node_type_t type) :
  _type(type),
  _doc(doc),
  _parent(nullptr)
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
  _children.push_back(child);
  child->_parent = this;
}

void
Node::remove_child(Node *child)
{
  _children.erase(std::remove(_children.begin(),
                              _children.end(),
                              child));
  child->_parent = nullptr;
}

Node *
Node::first_child() const
{
  if (_children.empty()) {
    return NULL;
  }
  return _children[0];
}

Node *
Node::parent() const
{
  return _parent;
}

Node *
Node::prev_sibling() const
{
  auto in_position = std::find(_parent->_children.rbegin(), _parent->_children.rend(), this);
  ++in_position;
  if (in_position == _parent->_children.rend()) {
    return NULL;
  } else {
    return *in_position;
  }
}

Node *
Node::next_sibling() const
{
  auto in_position = std::find(_parent->_children.begin(), _parent->_children.end(), this);
  ++in_position;
  if (in_position == _parent->_children.end()) {
    return NULL;
  } else {
    return *in_position;
  }
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
