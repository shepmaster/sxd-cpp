#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "document-internal.h"

void
Node::foreach_child(node_foreach_fn_t fn, gpointer user_data)
{
  Node *child = first_child_;
  while (child) {
    Node *next = child->next_sibling_;
    fn(child, user_data);
    child = next;
  }
}

void
Node::foreach_preceding_sibling(node_foreach_fn_t fn, gpointer user_data)
{
  Node *sibling = prev_sibling_;
  while (sibling) {
    Node *next = sibling->prev_sibling_;
    fn(sibling, user_data);
    sibling = next;
  }
}

void
Node::foreach_following_sibling(node_foreach_fn_t fn, gpointer user_data)
{
  Node *sibling = next_sibling_;
  while (sibling) {
    Node *next = sibling->next_sibling_;
    fn(sibling, user_data);
    sibling = next;
  }
}

void
Node::foreach_ancestor(node_foreach_fn_t fn, gpointer user_data)
{
  Node *parent;
  for (parent = parent_; parent; parent = parent->parent_) {
    fn(parent, user_data);
  }
}

static void
node_free_children(Node *node, gpointer user_data_unused)
{
  delete node;
}

Node::~Node()
{
  if (parent_) {
    parent_->remove_child(this);
  }

  foreach_child(node_free_children, nullptr);

  document_stop_managing_node(doc, this);
}

Node::Node(document_t *doc, node_type_t type) :
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

document_t *
Node::document()
{
  return doc;
}

static void
node_change_document_children(Node *node, gpointer doc_as_gp)
{
  document_t *doc = (document_t *)doc_as_gp;

  node->change_document(doc);
}

void
Node::change_document(document_t *doc)
{
  document_stop_managing_node(this->doc, this);

  this->doc = doc;

  foreach_child(node_change_document_children, doc);
}

static void
node_output_wrapper(Node *node, gpointer output_as_gp)
{
  node->output((output_t *)output_as_gp);
}

void
Node::output_children(output_t *output)
{
  foreach_child(node_output_wrapper, output);
}
