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

  xpath_parameters_t parameters;

  std::vector<XPathPredicate *> predicates;
  PotentialNodes potential;

  void setup(void)
  {
    alpha = test_helper_new_node(doc, "alpha");
    beta = test_helper_new_node(doc, "beta");
    gamma = test_helper_new_node(doc, "gamma");
    ns = nodeset_new_with_nodes(alpha, beta, gamma, NULL);
    potential.add_candidates(*ns);
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
  predicates.push_back(&predicate_true);

  Nodeset ns2 = potential.apply_predicates(predicates);
  CHECK_EQUAL(3, ns2.count());
}

TEST(predicate_value, false)
{
  PredicateValue predicate_false = PredicateValue::boolean(false);
  predicates.push_back(&predicate_false);

  Nodeset ns2 = potential.apply_predicates(predicates);
  CHECK_EQUAL(0, ns2.count());
}

TEST(predicate_value, implicit_position)
{
  PredicateValue numeric_value = PredicateValue::numeric(2);
  predicates.push_back(&numeric_value);

  Nodeset ns2 = potential.apply_predicates(predicates);
  CHECK_EQUAL(1, ns2.count());
  POINTERS_EQUAL(beta, ns2[0]);
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
