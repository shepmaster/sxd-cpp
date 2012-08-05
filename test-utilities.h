#ifndef TEST_UTILITIES_H
#define TEST_UTILITIES_H

#include <glib.h>
#include "element.h"
#include "nodeset.h"

Node *
test_helper_new_node(Document &doc, const char * const name);

Node *
test_helper_new_text_node(Document *doc, const char * const text);

class StringOutput : public Output {
public:
  StringOutput();
  ~StringOutput();
  void output(const char *format, ...);
  const char *string();
private:
  GString *string_;
};

Nodeset *
nodeset_new_with_nodes(Node *first, ...);

#endif
