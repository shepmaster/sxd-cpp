#include <iostream>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "document.h"
#include "xpath-functions.h"
#include "test-utilities.h"
#include "predicate-function.h"

TEST_GROUP(xpath_predicate)
{
  Document doc;
  Node *alpha;
  Nodeset *ns;

  xpath_parameters_t parameters;

  std::vector<XPathPredicate *> predicates;
  PotentialNodes potential;

  void setup(void)
  {
    alpha = test_helper_new_node(doc, "alpha");
    ns = nodeset_new_with_nodes(alpha, NULL);
    potential.add_candidates(*ns);
  }

  void teardown(void)
  {
    delete ns;
    delete alpha;
  }
};

TEST(xpath_predicate, predicate_fn_true)
{
  PredicateFunction pred(xpath_fn_true, parameters);
  predicates.push_back(&pred);

  Nodeset ns2 = potential.apply_predicates(predicates);
  CHECK_EQUAL(1, ns2.count());
}

TEST(xpath_predicate, predicate_fn_false)
{
  PredicateFunction pred(xpath_fn_false, parameters);
  predicates.push_back(&pred);

  Nodeset ns2 = potential.apply_predicates(predicates);
  CHECK_EQUAL(0, ns2.count());
}

TEST(xpath_predicate, predicate_fn_floor)
{
  xpath_result_t value;
  value.type = XPATH_RESULT_TYPE_NUMERIC;
  value.value.numeric = 1.3;
  parameters.push_back(value);

  PredicateFunction pred(xpath_fn_floor, parameters);
  predicates.push_back(&pred);

  Nodeset ns2 = potential.apply_predicates(predicates);
  CHECK_EQUAL(1, ns2.count());
}

TEST(xpath_predicate, predicate_fn_ceiling)
{
  xpath_result_t value;
  value.type = XPATH_RESULT_TYPE_NUMERIC;
  value.value.numeric = 0.1;
  parameters.push_back(value);

  PredicateFunction pred(xpath_fn_ceiling, parameters);
  predicates.push_back(&pred);

  Nodeset ns2 = potential.apply_predicates(predicates);
  CHECK_EQUAL(1, ns2.count());
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
