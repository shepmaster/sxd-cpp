#include <stdio.h>
#include <stdlib.h>

#include "nodeset.h"

unsigned int
Nodeset::count()
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
