#ifndef XPATH_H
#define XPATH_H

#include <string>
#include "nodeset.h"
#include "node.h"

class XPath : public NodeSelector {
public:
  XPath(std::string node_name);

  Nodeset select_nodes(Node &node);

private:
  std::string _node_name;
};

#endif
