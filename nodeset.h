#ifndef NODESET_H
#define NODESET_H

#include <vector>
#include "node.h"

class Nodeset {
public:
  unsigned int count();

  void add(Node *node);
  void add_nodeset(Nodeset &to_add);

  Node *operator[](unsigned int i);

private:
  std::vector<Node *> nodes;
};

#endif
