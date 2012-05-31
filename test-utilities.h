#ifndef TEST_UTILITIES_H
#define TEST_UTILITIES_H

#include <glib.h>

#include "element.h"

Node *
test_helper_new_node(Document &doc, const char * const name);

Node *
test_helper_new_text_node(document_t *doc, const char * const text);

class StringOutput : public Output {
public:
  StringOutput();
  ~StringOutput();
  void output(const char *format, ...);
  const char *string();
private:
  GString *string_;
};

nodeset_t *
nodeset_new_with_nodes(Node *first, ...);

#endif
