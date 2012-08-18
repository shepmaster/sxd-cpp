#ifndef XPATH_H
#define XPATH_H

#include <string>
#include "nodeset.h"
#include "node.h"

class XPath : public NodeSelector {
public:
  XPath(std::vector<std::string> node_names);

  Nodeset select_nodes(Node &node);

private:
  std::vector<std::string> _node_names;
};

#endif
