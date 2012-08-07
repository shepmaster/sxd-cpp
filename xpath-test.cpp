#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "xpath-internal.h"
#include "xpath-functions.h"
#include "test-utilities.h"

TEST_GROUP(xpath)
{};

typedef struct {
  Document *doc;
  Node *parent;
  Node *e;
  Node *tn;
} xpath_test_data_t;

static void
init_xpath_test(xpath_test_data_t *d)
{
  d->doc = new Document();
  d->parent = test_helper_new_node(*d->doc, "parent");
  d->e = test_helper_new_node(*d->doc, "child1");
  d->tn = test_helper_new_text_node(d->doc, "child2");

  d->parent->append_child(d->e);
  d->parent->append_child(d->tn);
}

static void
destroy_xpath_test(xpath_test_data_t *d)
{
  delete d->parent;
  delete d->doc;
}

static void
init_step(xpath_step_t *step)
{
  step->axis = XPATH_AXIS_CHILD;
  step->type = XPATH_NODE_TYPE_ELEMENT;
  step->name = NULL;
}

static void
destroy_step(xpath_step_t *step)
{
  if (step->name) free(step->name);
}

TEST(xpath, element)
{
  xpath_test_data_t d;
  Nodeset *ns;
  const Node *n;
  xpath_step_t step;

  init_xpath_test(&d);
  init_step(&step);

  ns = xpath_select_xpath_no_predicates(d.parent, &step);
  CHECK_EQUAL(1, ns->count());
  n = (*ns)[0];
  POINTERS_EQUAL(d.e, n);

  delete ns;
  destroy_xpath_test(&d);
}

TEST(xpath, text_node)
{
  xpath_test_data_t d;
  Nodeset *ns;
  const Node *n;
  xpath_step_t step;

  init_xpath_test(&d);
  init_step(&step);
  step.type = XPATH_NODE_TYPE_TEXT_NODE;

  ns = xpath_select_xpath_no_predicates(d.parent, &step);
  CHECK_EQUAL(1, ns->count());
  n = (*ns)[0];
  POINTERS_EQUAL(d.tn, n);

  delete ns;
  destroy_xpath_test(&d);
}

TEST(xpath, element_and_text_node)
{
  xpath_test_data_t d;
  Nodeset *ns;
  const Node *n;
  xpath_step_t step;

  init_xpath_test(&d);
  init_step(&step);
  step.type = XPATH_NODE_TYPE_ELEMENT | XPATH_NODE_TYPE_TEXT_NODE;

  ns = xpath_select_xpath_no_predicates(d.parent, &step);
  CHECK_EQUAL(2, ns->count());
  n = (*ns)[0];
  POINTERS_EQUAL(d.e, n);
  n = (*ns)[1];
  POINTERS_EQUAL(d.tn, n);

  delete ns;
  destroy_xpath_test(&d);
}

typedef struct {
  Document *doc;
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
} xpath_axis_test_t;

static void
init_xpath_axis_test(xpath_axis_test_t *d)
{
  d->doc = new Document();
  d->alpha = test_helper_new_node(*d->doc, "alpha");
  d->one = test_helper_new_node(*d->doc, "one");
  d->a = test_helper_new_node(*d->doc, "a");
  d->b = test_helper_new_node(*d->doc, "b");
  d->c = test_helper_new_node(*d->doc, "c");
  d->d = test_helper_new_node(*d->doc, "d");
  d->two = test_helper_new_node(*d->doc, "two");
  d->w = test_helper_new_node(*d->doc, "w");
  d->x = test_helper_new_node(*d->doc, "x");
  d->y = test_helper_new_node(*d->doc, "y");
  d->z = test_helper_new_node(*d->doc, "z");

  d->alpha->append_child(d->one);
  d->one->append_child(d->a);
  d->one->append_child(d->b);
  d->one->append_child(d->c);
  d->one->append_child(d->d);
  d->alpha->append_child(d->two);
  d->two->append_child(d->w);
  d->two->append_child(d->x);
  d->two->append_child(d->y);
  d->two->append_child(d->z);
}

static void
destroy_xpath_axis_test(xpath_axis_test_t *d)
{
  delete d->alpha;
  delete d->doc;
}

#define CHECK_nodeset_item(_node, _nodeset, _index) \
  POINTERS_EQUAL(_node, (*_nodeset)[_index])

TEST_GROUP(xpath_axis)
{
  Nodeset *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  void setup(void)
  {
    init_xpath_axis_test(&d);
    init_step(&step);
  }

  void teardown(void)
  {
    delete ns;
    destroy_xpath_axis_test(&d);
  }
};

TEST(xpath_axis, axis_self)
{
  step.axis = XPATH_AXIS_SELF;

  ns = xpath_select_xpath_no_predicates(d.b, &step);
  CHECK_EQUAL(1, ns->count());
  CHECK_nodeset_item(d.b, ns, 0);
}

TEST(xpath_axis, axis_parent)
{
  step.axis = XPATH_AXIS_PARENT;

  ns = xpath_select_xpath_no_predicates(d.b, &step);
  CHECK_EQUAL(1, ns->count());
  CHECK_nodeset_item(d.one, ns, 0);
}

TEST(xpath_axis, axis_following_sibling)
{
  step.axis = XPATH_AXIS_FOLLOWING_SIBLING;

  ns = xpath_select_xpath_no_predicates(d.b, &step);
  CHECK_EQUAL(2, ns->count());
  CHECK_nodeset_item(d.c, ns, 0);
  CHECK_nodeset_item(d.d, ns, 1);
}

TEST(xpath_axis, axis_preceding_sibling)
{
  step.axis = XPATH_AXIS_PRECEDING_SIBLING;

  ns = xpath_select_xpath_no_predicates(d.d, &step);
  CHECK_EQUAL(3, ns->count());
  CHECK_nodeset_item(d.c, ns, 0);
  CHECK_nodeset_item(d.b, ns, 1);
  CHECK_nodeset_item(d.a, ns, 2);
}

TEST(xpath_axis, axis_descendant)
{
  step.axis = XPATH_AXIS_DESCENDANT;

  ns = xpath_select_xpath_no_predicates(d.alpha, &step);
  CHECK_EQUAL(10, ns->count());
  CHECK_nodeset_item(d.one, ns, 0);
  CHECK_nodeset_item(d.a, ns, 1);
  CHECK_nodeset_item(d.b, ns, 2);
  CHECK_nodeset_item(d.c, ns, 3);
  CHECK_nodeset_item(d.d, ns, 4);
  CHECK_nodeset_item(d.two, ns, 5);
  CHECK_nodeset_item(d.w, ns, 6);
  CHECK_nodeset_item(d.x, ns, 7);
  CHECK_nodeset_item(d.y, ns, 8);
  CHECK_nodeset_item(d.z, ns, 9);
}

TEST(xpath_axis, axis_descendant_or_self)
{
  step.axis = XPATH_AXIS_DESCENDANT_OR_SELF;

  ns = xpath_select_xpath_no_predicates(d.alpha, &step);
  CHECK_EQUAL(11, ns->count());
  CHECK_nodeset_item(d.alpha, ns, 0);
  CHECK_nodeset_item(d.one, ns, 1);
  CHECK_nodeset_item(d.a, ns, 2);
  CHECK_nodeset_item(d.b, ns, 3);
  CHECK_nodeset_item(d.c, ns, 4);
  CHECK_nodeset_item(d.d, ns, 5);
  CHECK_nodeset_item(d.two, ns, 6);
  CHECK_nodeset_item(d.w, ns, 7);
  CHECK_nodeset_item(d.x, ns, 8);
  CHECK_nodeset_item(d.y, ns, 9);
  CHECK_nodeset_item(d.z, ns, 10);
}

TEST(xpath_axis, axis_ancestor)
{
  step.axis = XPATH_AXIS_ANCESTOR;

  ns = xpath_select_xpath_no_predicates(d.b, &step);
  CHECK_EQUAL(2, ns->count());
  CHECK_nodeset_item(d.one, ns, 0);
  CHECK_nodeset_item(d.alpha, ns, 1);
}

TEST(xpath_axis, axis_ancestor_or_self)
{
  step.axis = XPATH_AXIS_ANCESTOR_OR_SELF;

  ns = xpath_select_xpath_no_predicates(d.c, &step);
  CHECK_EQUAL(3, ns->count());
  CHECK_nodeset_item(d.c, ns, 0);
  CHECK_nodeset_item(d.one, ns, 1);
  CHECK_nodeset_item(d.alpha, ns, 2);
}

TEST(xpath_axis, axis_following)
{
  step.axis = XPATH_AXIS_FOLLOWING;

  ns = xpath_select_xpath_no_predicates(d.c, &step);
  CHECK_EQUAL(6, ns->count());
  CHECK_nodeset_item(d.d, ns, 0);
  CHECK_nodeset_item(d.two, ns, 1);
  CHECK_nodeset_item(d.w, ns, 2);
  CHECK_nodeset_item(d.x, ns, 3);
  CHECK_nodeset_item(d.y, ns, 4);
  CHECK_nodeset_item(d.z, ns, 5);
}

TEST(xpath_axis, axis_preceding)
{
  step.axis = XPATH_AXIS_PRECEDING;

  ns = xpath_select_xpath_no_predicates(d.x, &step);
  CHECK_EQUAL(6, ns->count());
  CHECK_nodeset_item(d.w, ns, 0);
  CHECK_nodeset_item(d.one, ns, 1);
  CHECK_nodeset_item(d.a, ns, 2);
  CHECK_nodeset_item(d.b, ns, 3);
  CHECK_nodeset_item(d.c, ns, 4);
  CHECK_nodeset_item(d.d, ns, 5);
}

TEST(xpath, two_step)
{
  Nodeset *ns;
  xpath_step_t step;
  std::vector<xpath_step_t> steps;
  xpath_axis_test_t d;
  unsigned int i;

  init_xpath_axis_test(&d);

  init_step(&step);
  step.name = strdup("one");
  steps.push_back(step);
  step.name = strdup("c");
  steps.push_back(step);

  ns = xpath_select_xpath_steps(d.alpha, steps);
  CHECK_EQUAL(1, ns->count());
  CHECK_nodeset_item(d.c, ns, 0);

  for (i = 0; i < steps.size(); i++) {
    xpath_step_t *x = &steps[i];
    destroy_step(x);
  }

  delete ns;
  destroy_xpath_axis_test(&d);
}

TEST_GROUP(xpath_predicate)
{
  Nodeset *ns;
  xpath_step_t step;
  xpath_axis_test_t d;

  void setup(void)
  {
    init_xpath_axis_test(&d);
    init_step(&step);
  }

  void teardown(void)
  {
    delete ns;
    destroy_xpath_axis_test(&d);
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

  step.predicates.push_back(&pred);

  ns = nodeset_new_with_nodes(d.alpha, NULL);
  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(1, ns->count());
}

#define CHECK_nodeset_element_name(_nodeset, _index, _name)  \
  {                                                          \
    Element *__e;                                            \
    Node *__n;                                               \
    __n = (*_nodeset)[_index];                               \
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
  Nodeset *nodes;

  children[0] = test_helper_new_node(doc, "one");
  children[1] = test_helper_new_node(doc, "two");
  children[2] = test_helper_new_node(doc, "one");
  children[3] = test_helper_new_node(doc, "four");

  parent->append_child(children[0]);
  parent->append_child(children[1]);
  parent->append_child(children[2]);
  parent->append_child(children[3]);

  nodes = XPathProcessor(parent).apply(name);

  CHECK_EQUAL(2, nodes->count());
  CHECK_nodeset_element_name(nodes, 0, name);
  CHECK_nodeset_element_name(nodes, 1, name);

  delete nodes;
  delete parent;
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
