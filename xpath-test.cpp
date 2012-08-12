#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "axis-child.h"
#include "xpath-functions.h"

#include "xpath-axis-test-data.h"
#include "test-utilities.h"

#include "xpath-internal.h"

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

class MockNodeTest : public XPathNodeTest {
public:
  MockNodeTest(bool should_include) :
    should_include(should_include)
  {
  }

  bool include_node(Node &node) {
    ++call_count;
    return should_include;
  }

  bool should_include;
  unsigned int call_count = 0;
};

TEST(xpath, applies_tests_for_each_node)
{
  XPathStep step(new AxisChild());
  MockNodeTest test(false);
  step.tests.push_back(&test);

  Nodeset ns = step.select_without_predicates(d.parent);

  CHECK_EQUAL(2, test.call_count);
}

TEST(xpath, applies_multiple_tests)
{
  XPathStep step(new AxisChild());
  MockNodeTest test1(false);
  MockNodeTest test2(false);
  step.tests.push_back(&test1);
  step.tests.push_back(&test2);

  Nodeset ns = step.select_without_predicates(d.parent);

  CHECK_EQUAL(2, test1.call_count);
  CHECK_EQUAL(2, test2.call_count);
}

TEST(xpath, stops_applying_tests_after_success)
{
  XPathStep step(new AxisChild());
  MockNodeTest test1(true);
  MockNodeTest test2(false);
  step.tests.push_back(&test1);
  step.tests.push_back(&test2);

  Nodeset ns = step.select_without_predicates(d.parent);

  CHECK_EQUAL(2, test1.call_count);
  CHECK_EQUAL(0, test2.call_count);
}

#define CHECK_nodeset_item(_node, _nodeset, _index) \
  POINTERS_EQUAL(_node, _nodeset[_index])

#include "node-test-named-element.h"

TEST(xpath, two_step)
{
  std::vector<XPathStep> steps;
  XPathAxisTestData d;

  XPathStep first_step(new AxisChild());
  first_step.tests.push_back(new NodeTestNamedElement("one"));
  steps.push_back(first_step);

  XPathStep second_step(new AxisChild());
  second_step.tests.push_back(new NodeTestNamedElement("c"));
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
  Node *parent = doc.new_element("parent");
  parent->append_child(doc.new_element("one"));
  parent->append_child(doc.new_element("two"));
  parent->append_child(doc.new_element("one"));
  parent->append_child(doc.new_element("four"));

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
