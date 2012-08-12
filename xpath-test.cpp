#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "xpath-internal.h"
#include "xpath-functions.h"
#include "axis-ancestor.h"
#include "axis-ancestor-or-self.h"
#include "axis-child.h"
#include "axis-descendant.h"
#include "axis-descendant-or-self.h"
#include "axis-following.h"
#include "axis-following-sibling.h"
#include "axis-parent.h"
#include "axis-preceding.h"
#include "axis-preceding-sibling.h"
#include "axis-self.h"
#include "test-utilities.h"

struct XPathTestData {
  XPathTestData()
{
  parent = doc.new_element("parent");
  e = doc.new_element("child1");
  tn = doc.new_text_node("child2");

  parent->append_child(e);
  parent->append_child(tn);
}

~XPathTestData()
{
  delete parent;
}

  Document doc;
  Node *parent;
  Node *e;
  Node *tn;
};

TEST_GROUP(xpath)
{
  XPathTestData d;
};

TEST(xpath, element)
{
  XPathStep step(new AxisChild());

  step.tests.push_back(new ElementTest());

  Nodeset ns = step.select_without_predicates(d.parent);
  CHECK_EQUAL(1, ns.count());
  POINTERS_EQUAL(d.e, ns[0]);
}

TEST(xpath, text_node)
{
  XPathStep step(new AxisChild());

  step.tests.push_back(new TextTest());

  Nodeset ns = step.select_without_predicates(d.parent);
  CHECK_EQUAL(1, ns.count());
  POINTERS_EQUAL(d.tn, ns[0]);
}

TEST(xpath, element_and_text_node)
{
  XPathStep step(new AxisChild());

  step.tests.push_back(new ElementTest());
  step.tests.push_back(new TextTest());

  Nodeset ns = step.select_without_predicates(d.parent);
  CHECK_EQUAL(2, ns.count());
  POINTERS_EQUAL(d.e, ns[0]);
  POINTERS_EQUAL(d.tn, ns[1]);
}

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

#define CHECK_nodeset_item(_node, _nodeset, _index) \
  POINTERS_EQUAL(_node, _nodeset[_index])

TEST_GROUP(xpath_axis)
{
  XPathAxisTestData d;
};

class RememberingVisitor {
public:
  void operator() (Node *node) {
    seen_nodes.push_back(node);
  }

  unsigned int nodes_seen() {
    return seen_nodes.size();
  }

  Node *operator[] (unsigned int idx) {
    return seen_nodes[idx];
  }

  std::vector<Node *> seen_nodes;
};

TEST(xpath_axis, axis_self)
{
  AxisSelf axis;
  RememberingVisitor visitor;

  axis.traverse(d.b, std::ref(visitor));

  CHECK_EQUAL(1, visitor.nodes_seen());
  CHECK_EQUAL(d.b, visitor[0]);
}

TEST(xpath_axis, axis_parent)
{
  AxisParent axis;
  RememberingVisitor visitor;

  axis.traverse(d.b, std::ref(visitor));

  CHECK_EQUAL(1, visitor.nodes_seen());
  CHECK_EQUAL(d.one, visitor[0]);
}

TEST(xpath_axis, axis_following_sibling)
{
  AxisFollowingSibling axis;
  RememberingVisitor visitor;

  axis.traverse(d.b, std::ref(visitor));

  CHECK_EQUAL(2, visitor.nodes_seen());
  CHECK_EQUAL(d.c, visitor[0]);
  CHECK_EQUAL(d.d, visitor[1]);
}

TEST(xpath_axis, axis_preceding_sibling)
{
  AxisPrecedingSibling axis;
  RememberingVisitor visitor;

  axis.traverse(d.d, std::ref(visitor));

  CHECK_EQUAL(3, visitor.nodes_seen());
  CHECK_EQUAL(d.c, visitor[0]);
  CHECK_EQUAL(d.b, visitor[1]);
  CHECK_EQUAL(d.a, visitor[2]);
}

TEST(xpath_axis, axis_descendant)
{
  AxisDescendant axis;
  RememberingVisitor visitor;

  axis.traverse(d.alpha, std::ref(visitor));

  CHECK_EQUAL(10, visitor.nodes_seen());
  CHECK_EQUAL(d.one, visitor[0]);
  CHECK_EQUAL(d.a, visitor[1]);
  CHECK_EQUAL(d.b, visitor[2]);
  CHECK_EQUAL(d.c, visitor[3]);
  CHECK_EQUAL(d.d, visitor[4]);
  CHECK_EQUAL(d.two, visitor[5]);
  CHECK_EQUAL(d.w, visitor[6]);
  CHECK_EQUAL(d.x, visitor[7]);
  CHECK_EQUAL(d.y, visitor[8]);
  CHECK_EQUAL(d.z, visitor[9]);
}

TEST(xpath_axis, axis_descendant_or_self)
{
  AxisDescendantOrSelf axis;
  RememberingVisitor visitor;

  axis.traverse(d.alpha, std::ref(visitor));

  CHECK_EQUAL(11, visitor.nodes_seen());
  CHECK_EQUAL(d.alpha, visitor[0]);
  CHECK_EQUAL(d.one, visitor[1]);
  CHECK_EQUAL(d.a, visitor[2]);
  CHECK_EQUAL(d.b, visitor[3]);
  CHECK_EQUAL(d.c, visitor[4]);
  CHECK_EQUAL(d.d, visitor[5]);
  CHECK_EQUAL(d.two, visitor[6]);
  CHECK_EQUAL(d.w, visitor[7]);
  CHECK_EQUAL(d.x, visitor[8]);
  CHECK_EQUAL(d.y, visitor[9]);
  CHECK_EQUAL(d.z, visitor[10]);
}

TEST(xpath_axis, axis_ancestor)
{
  AxisAncestor axis;
  RememberingVisitor visitor;

  axis.traverse(d.b, std::ref(visitor));

  CHECK_EQUAL(2, visitor.nodes_seen());
  CHECK_EQUAL(d.one, visitor[0]);
  CHECK_EQUAL(d.alpha, visitor[1]);
}

TEST(xpath_axis, axis_ancestor_or_self)
{
  AxisAncestorOrSelf axis;
  RememberingVisitor visitor;

  axis.traverse(d.c, std::ref(visitor));

  CHECK_EQUAL(3, visitor.nodes_seen());
  CHECK_EQUAL(d.c, visitor[0]);
  CHECK_EQUAL(d.one, visitor[1]);
  CHECK_EQUAL(d.alpha, visitor[2]);
}

TEST(xpath_axis, axis_following)
{
  AxisFollowing axis;
  RememberingVisitor visitor;

  axis.traverse(d.c, std::ref(visitor));

  CHECK_EQUAL(6, visitor.nodes_seen());
  CHECK_EQUAL(d.d, visitor[0]);
  CHECK_EQUAL(d.two, visitor[1]);
  CHECK_EQUAL(d.w, visitor[2]);
  CHECK_EQUAL(d.x, visitor[3]);
  CHECK_EQUAL(d.y, visitor[4]);
  CHECK_EQUAL(d.z, visitor[5]);
}

TEST(xpath_axis, axis_preceding)
{
  AxisPreceding axis;
  RememberingVisitor visitor;

  axis.traverse(d.x, std::ref(visitor));

  CHECK_EQUAL(6, visitor.nodes_seen());
  CHECK_EQUAL(d.w, visitor[0]);
  CHECK_EQUAL(d.one, visitor[1]);
  CHECK_EQUAL(d.a, visitor[2]);
  CHECK_EQUAL(d.b, visitor[3]);
  CHECK_EQUAL(d.c, visitor[4]);
  CHECK_EQUAL(d.d, visitor[5]);
}

TEST(xpath, two_step)
{
  std::vector<XPathStep> steps;
  XPathAxisTestData d;

  XPathStep first_step(new AxisChild());
  first_step.tests.push_back(new NamedElementTest("one"));
  steps.push_back(first_step);

  XPathStep second_step(new AxisChild());
  second_step.tests.push_back(new NamedElementTest("c"));
  steps.push_back(second_step);

  Nodeset ns = XPathProcessor(d.alpha).select_steps(steps);
  CHECK_EQUAL(1, ns.count());
  CHECK_nodeset_item(d.c, ns, 0);
}

TEST_GROUP(xpath_predicate)
{
  Nodeset *ns;
  XPathAxisTestData d;

  void setup(void)
  {
  }

  void teardown(void)
  {
    delete ns;
  }
};

#include "predicate-value.h"
#include "predicate-function.h"
#include "predicate-equals.h"
// Where does this test really belong?

TEST(xpath_predicate, predicate_position_1)
{
  xpath_parameters_t parameters;
  PredicateValue pred_val_1 = PredicateValue::numeric(1);
  PredicateFunction pred_fn_position(xpath_fn_position, parameters);
  PredicateEquals pred(pred_val_1, pred_fn_position);

  ns = nodeset_new_with_nodes(d.alpha, NULL);
  PotentialNodes potential;
  potential.add_candidates(*ns);

  std::vector<XPathPredicate *> predicates;
  predicates.push_back(&pred);

  Nodeset ns2 = potential.apply_predicates(predicates);
  CHECK_EQUAL(1, ns2.count());
}

#define CHECK_nodeset_element_name(_nodeset, _index, _name)  \
  {                                                          \
    Element *__e;                                            \
    Node *__n;                                               \
    __n = _nodeset[_index];                                  \
    CHECK_EQUAL(NODE_TYPE_ELEMENT, __n->type());             \
    __e = (Element *)__n;                                    \
    STRCMP_EQUAL(_name, __e->name());                        \
  }

TEST(xpath, apply_element)
{
  const char * const name = "one";

  Document doc;
  Node *parent = test_helper_new_node(doc, "parent");
  Node *children[4];

  children[0] = test_helper_new_node(doc, "one");
  children[1] = test_helper_new_node(doc, "two");
  children[2] = test_helper_new_node(doc, "one");
  children[3] = test_helper_new_node(doc, "four");

  parent->append_child(children[0]);
  parent->append_child(children[1]);
  parent->append_child(children[2]);
  parent->append_child(children[3]);

  Nodeset nodes = XPathProcessor(parent).apply(name);

  CHECK_EQUAL(2, nodes.count());
  CHECK_nodeset_element_name(nodes, 0, name);
  CHECK_nodeset_element_name(nodes, 1, name);

  delete parent;
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
