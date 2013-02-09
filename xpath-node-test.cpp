#include "xpath-node-test.h"

std::ostream &
operator<<(std::ostream &strm, const XPathNodeTest &node_test)
{
  return node_test.to_string(strm);
}
