#ifndef XPATH_H
#define XPATH_H

#include <string>
#include "nodeset.h"
#include "node.h"

#include "xpath-step.h"

class XPath : public NodeSelector {
public:
  XPath();
  XPath(std::vector<std::unique_ptr<XPathStep>> &&steps);

  Nodeset select_nodes(Node *node);

private:
  std::vector<std::unique_ptr<XPathStep>> _steps;
};

#endif
