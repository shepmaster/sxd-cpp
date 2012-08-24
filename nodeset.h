#ifndef NODESET_H
#define NODESET_H

#include <vector>
#include "node.h"

class Nodeset {
private:
  typedef std::vector<Node *> Storage;
  Storage nodes;
public:
  unsigned int count() const;

  void add(Node *node);
  void add_nodeset(Nodeset &to_add);

  Node *operator[](unsigned int i) const;

  bool operator ==(const Nodeset &other) const;
  bool operator !=(const Nodeset &other) const;

  // Iterator methods
  typedef Storage::iterator iterator;
  typedef Storage::const_iterator const_iterator;
  size_t size() const { return count(); }
  bool empty() const { return nodes.empty(); }
  const_iterator begin() const { return nodes.begin(); }
  const_iterator end() const { return nodes.end(); }
  typedef Storage::value_type value_type;
};

std::ostream &
operator<<(std::ostream &os, const Nodeset &nodeset);

#endif
