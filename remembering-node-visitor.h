#ifndef REMEMBERING_NODE_VISITOR_H
#define REMEMBERING_NODE_VISITOR_H

#include <vector>
#include "node.h"

class RememberingNodeVisitor {
public:
  void operator() (Node *node);

  unsigned int nodes_seen();
  Node *operator[] (unsigned int idx);

private:
  std::vector<Node *> seen_nodes;
};

#endif
