#ifndef XPATH_H
#define XPATH_H

#include "nodeset.h"

class XPath {
public:
  XPath(std::string xpath);
  Nodeset select(Node *node);
};

#endif
