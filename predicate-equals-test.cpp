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

TEST(predicate_equal, true)
{
  PredicateEquals pred(g_predicate_true, g_predicate_true);
  predicates.push_back(&pred);

  Nodeset ns2 = potential.apply_predicates(predicates);
  CHECK_EQUAL(1, ns2.count());
}

TEST(predicate_equal, false)
{
  PredicateEquals pred(g_predicate_true, g_predicate_false);
  predicates.push_back(&pred);

  Nodeset ns2 = potential.apply_predicates(predicates);
  CHECK_EQUAL(0, ns2.count());
}

int
main(int argc, char **argv)
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
