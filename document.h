#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <glib.h>

#include "node.h"
#include "element.h"
#include "text-node.h"
#include "comment.h"
#include "intern.h"

class Document {
public:
  static Document *parse(const char *input, GError **error);
  Document();
  ~Document();

  Element *new_element(const char * const name);
  TextNode *new_text_node(const char * const text);
  Comment *new_comment(const char * const text);

  void manage_node(Node *node);
  void stop_managing_node(Node *node);
  unsigned int managed_node_count();

  Element *root();

  const char *intern(const char * const string);
private:
  Element *root_;
  intern_t *dict;
  unsigned int managed_node_count_;
};

typedef class Document document_t;

#define DOCUMENT_PARSE_ERROR document_parse_error_quark()

GQuark
document_parse_error_quark(void);

typedef enum {
  DOCUMENT_PARSE_ERROR_FAILED
} DocumentParseError;

#endif
