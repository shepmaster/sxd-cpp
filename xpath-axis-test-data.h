#ifndef XPATH_AXIS_TEST_DATA_H
#define XPATH_AXIS_TEST_DATA_H

#include "document.h"
#include "node.h"

struct XPathAxisTestData {
  XPathAxisTestData() {
    alpha = doc.new_element("alpha");
    one = doc.new_element("one");
    a = doc.new_element("a");
    b = doc.new_element("b");
    c = doc.new_element("c");
    d = doc.new_element("d");
    two = doc.new_element("two");
    w = doc.new_element("w");
    x = doc.new_element("x");
    y = doc.new_element("y");
    z = doc.new_element("z");

    alpha->append_child(one);
    one->append_child(a);
    one->append_child(b);
    one->append_child(c);
    one->append_child(d);
    alpha->append_child(two);
    two->append_child(w);
    two->append_child(x);
    two->append_child(y);
    two->append_child(z);
  }

  ~XPathAxisTestData() {
    delete alpha;
  }

  Document doc;
  Node *alpha;
  Node *one;
  Node *a;
  Node *b;
  Node *c;
  Node *d;
  Node *two;
  Node *w;
  Node *x;
  Node *y;
  Node *z;
};

#endif
