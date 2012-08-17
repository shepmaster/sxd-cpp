#include <iostream>

#include "document.h"
#include "nodeset.h"

void
Node::foreach_child(foreach_fn_t fn)
{
  Node *child = first_child_;
  while (child) {
    Node *next = child->next_sibling_;
    fn(child);
    child = next;
  }
}

void
Node::foreach_preceding_sibling(foreach_fn_t fn)
{
  Node *sibling = prev_sibling_;
  while (sibling) {
    Node *next = sibling->prev_sibling_;
    fn(sibling);
    sibling = next;
  }
}

void
Node::foreach_following_sibling(foreach_fn_t fn)
{
  Node *sibling = next_sibling_;
  while (sibling) {
    Node *next = sibling->next_sibling_;
    fn(sibling);
    sibling = next;
  }
}

void
Node::foreach_ancestor(foreach_fn_t fn)
{
  Node *parent;
  for (parent = parent_; parent; parent = parent->parent_) {
    fn(parent);
  }
}

struct FreeChildren {
  virtual void operator() (Node *node) { delete node; }
};

Node::~Node()
{
  if (parent_) {
    parent_->remove_child(this);
  }

  foreach_child(FreeChildren());

  doc->stop_managing_node(this);
}

Node::Node(Document *doc, node_type_t type) :
  type_(type), doc(doc),
  parent_(nullptr),
  first_child_(nullptr),
  prev_sibling_(nullptr),
  next_sibling_(nullptr)
{
}

node_type_t
Node::type()
{
  return type_;
}

Node *
node_cast_to_node(Node *n)
{
  return n;
}

void
Node::append_child(Node *child)
{
  Node *my_child = first_child_;
  if (my_child) {
    while (my_child->next_sibling_) {
      my_child = my_child->next_sibling_;
    }
    my_child->insert_next_sibling(child);
  } else {
    first_child_ = child;
  }
  child->parent_ = this;
}

void
Node::remove_child(Node *child)
{
  if (child->prev_sibling_) {
    child->prev_sibling_->next_sibling_ = child->next_sibling_;
  }

  if (child->next_sibling_) {
    child->next_sibling_->prev_sibling_ = child->prev_sibling_;
  }

  if (child->parent_ && child->parent_->first_child_ == child) {
    child->parent_->first_child_ = child->next_sibling_;
  }
  child->parent_ = nullptr;
}

Node *
Node::first_child()
{
  return first_child_;
}

Node *
Node::parent()
{
  return parent_;
}

Node *
Node::prev_sibling()
{
  return prev_sibling_;
}

Node *
Node::next_sibling()
{
  return next_sibling_;
}

void
Node::insert_next_sibling(Node *new_sibling)
{
  if (next_sibling_) {
    next_sibling_->prev_sibling_ = new_sibling;
  }

  new_sibling->prev_sibling_ = this;
  new_sibling->next_sibling_ = next_sibling_;

  next_sibling_ = new_sibling;
}

Document *
Node::document()
{
  return doc;
}

struct ChangeDocumentChildren {
  ChangeDocumentChildren(Document *doc) : doc(doc) {}
  void operator()(Node *node) {  node->change_document(doc); }
  Document *doc;
};

void
Node::change_document(Document *doc)
{
  this->doc->stop_managing_node(this);

  this->doc = doc;

  foreach_child(ChangeDocumentChildren(doc));
}

struct OutputWrapper {
  OutputWrapper(Output &output) : output(output) {}
  void operator() (Node *node) { node->output(output); }
  Output &output;
};

void
Node::output_children(Output &output)
{
  foreach_child(OutputWrapper(output));
}

Nodeset
Node::select_nodes(NodeSelector &selector)
{
  return selector.select_nodes(*this);
}
