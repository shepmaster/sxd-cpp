#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "test-utilities.h"
#include "predicate-value.h"

TEST_GROUP(predicate_value)
{
  Document doc;
  Node *alpha;
  Node *beta;
  Node *gamma;
  Nodeset *ns;

  xpath_step_t step;

  xpath_parameters_t parameters;

  void setup(void)
  {
    alpha = test_helper_new_node(doc, "alpha");
    beta = test_helper_new_node(doc, "beta");
    gamma = test_helper_new_node(doc, "gamma");
    ns = nodeset_new_with_nodes(alpha, NULL);

    step.axis = XPATH_AXIS_CHILD;
    step.type = XPATH_NODE_TYPE_ELEMENT;
    step.name = NULL;
  }

  void teardown(void)
  {
    delete ns;
    delete alpha;
    delete beta;
    delete gamma;
  }
};

TEST(predicate_value, true)
{
  PredicateValue predicate_true = PredicateValue::boolean(true);
  step.predicates.push_back(&predicate_true);

  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(1, ns->count());
}

TEST(predicate_value, false)
{
  PredicateValue predicate_false = PredicateValue::boolean(false);
  step.predicates.push_back(&predicate_false);

  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(0, ns->count());
}

TEST(predicate_value, implicit_position)
{
  PredicateValue numeric_value = PredicateValue::numeric(2);
  step.predicates.push_back(&numeric_value);

  Nodeset *ns2 = nodeset_new_with_nodes(alpha, beta, gamma, NULL);

  ns2 = xpath_apply_predicates(ns2, &step);
  CHECK_EQUAL(1, ns2->count());
  POINTERS_EQUAL(beta, (*ns2)[0]);

  delete ns2;
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
