#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "test-utilities.h"
#include "predicate-value.h"
#include "predicate-equals.h"

TEST_GROUP(predicate_equal)
{
  PredicateValue g_predicate_true = PredicateValue::boolean(true);
  PredicateValue g_predicate_false = PredicateValue::boolean(false);

  Document doc;
  Node *alpha;
  Nodeset *ns;

  xpath_step_t step;
  xpath_parameters_t parameters;

  void setup(void)
  {
    alpha = test_helper_new_node(doc, "alpha");
    ns = nodeset_new_with_nodes(alpha, NULL);

    step.axis = XPATH_AXIS_CHILD;
    step.type = XPATH_NODE_TYPE_ELEMENT;
    step.name = NULL;
  }

  void teardown(void)
  {
    delete ns;
    delete alpha;
  }
};

TEST(predicate_equal, true)
{
  PredicateEquals pred(g_predicate_true, g_predicate_true);
  step.predicates.push_back(&pred);

  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(1, ns->count());
}

TEST(predicate_equal, false)
{
  PredicateEquals pred(g_predicate_true, g_predicate_false);
  step.predicates.push_back(&pred);

  ns = xpath_apply_predicates(ns, &step);
  CHECK_EQUAL(0, ns->count());
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
