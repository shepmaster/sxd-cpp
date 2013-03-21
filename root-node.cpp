#include "root-node.h"

RootNode::RootNode(Document *doc) :
  Node(doc, NODE_TYPE_ROOT_NODE)
{}

void
RootNode::output(Output &output) const
{}

std::ostream &
RootNode::to_stream(std::ostream& os) const
{
  return os << "Element <" << this << ">";
}
