#include "xpath-axis.h"

std::ostream &
operator<<(std::ostream &strm, const XPathAxis &axis)
{
  return axis.to_string(strm);
}
