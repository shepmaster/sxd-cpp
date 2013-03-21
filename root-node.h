#ifndef ROOT_NODE_H
#define ROOT_NODE_H

#include "node.h"

class RootNode : public Node {
public:
  RootNode(Document *doc);
  void output(Output &output) const;
  std::ostream& to_stream(std::ostream& os) const;
};

#endif
