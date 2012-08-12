#include "remembering-node-visitor.h"

void
RememberingNodeVisitor::operator() (Node *node) {
  seen_nodes.push_back(node);
}

unsigned int
RememberingNodeVisitor::nodes_seen() {
  return seen_nodes.size();
}

Node *
RememberingNodeVisitor::operator[] (unsigned int idx) {
  return seen_nodes[idx];
}
