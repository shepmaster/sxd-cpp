#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <glib.h>

#include "node.h"
#include "element.h"
#include "text-node.h"
#include "root-node.h"
#include "comment.h"

#include <set>

class Document {
public:
  static Document *parse(const char *input, GError **error);
  Document();

  Element *new_element(const std::string name);
  Element *new_element(const std::string namespace_uri, const std::string name);
  Element *new_element(const QName qname);
  Attribute *new_attribute(const Element &element,
                           const std::string name, const std::string value);
  TextNode *new_text_node(const std::string text);
  Comment *new_comment(const std::string text);

  RootNode *root();

private:
  RootNode *root_;
  std::set<std::unique_ptr<Node>> _nodes;
};

#define DOCUMENT_PARSE_ERROR document_parse_error_quark()

GQuark
document_parse_error_quark(void);

typedef enum {
  DOCUMENT_PARSE_ERROR_FAILED
} DocumentParseError;

#endif
