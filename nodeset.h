#ifndef NODESET_H
#define NODESET_H

#include <vector>
#include "node.h"

class Nodeset {
public:
  unsigned int count() const;

  void add(Node *node);
  void add_nodeset(Nodeset &to_add);

  Node *operator[](unsigned int i) const;

  bool operator ==(const Nodeset &other) const;
  bool operator !=(const Nodeset &other) const;

private:
  std::vector<Node *> nodes;
};

std::ostream &
operator<<(std::ostream &os, const Nodeset &nodeset);

#endif
