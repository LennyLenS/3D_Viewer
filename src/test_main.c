#include "tests/unit.h"

void run_test_case(Suite *testcase);
void run_all_tests();

int main() {
  run_all_tests();
  return 0;
}

void run_test_case(Suite *testcase) {
  static int count = 1;
  printf("TEST CASE №%d\n", count++);

  SRunner *sr;
  sr = srunner_create(testcase);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  srunner_free(sr);
}

void run_all_tests() {
  Suite *list_cases[] = {
      suite_parser(),
  };
  for (int i = 0; i < (int)(sizeof(list_cases) / sizeof(Suite *)); i++)
    run_test_case(list_cases[i]);
}
