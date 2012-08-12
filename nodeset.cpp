#include <iostream>

#include "nodeset.h"

unsigned int
Nodeset::count() const
{
  return nodes.size();
}

void
Nodeset::add(Node *node)
{
  nodes.push_back(node);
}

void
Nodeset::add_nodeset(Nodeset &to_add)
{
  nodes.insert(nodes.end(), to_add.nodes.begin(), to_add.nodes.end());
}

Node *
Nodeset::operator[](unsigned int i)
{
  return nodes[i];
}

bool
Nodeset::operator ==(const Nodeset &other) const
{
  return nodes == other.nodes;
};

bool
Nodeset::operator !=(const Nodeset &other) const
{
  return !(*this == other);
}

std::ostream &
operator<<(std::ostream &os, const Nodeset &nodeset) {
  return os << "Nodeset contains " << nodeset.count() << " nodes";
}
