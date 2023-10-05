#include "unit.h"

START_TEST(parser1) {
  data *res = parser("./tests/test.test");
  ck_assert_int_eq(res->correct, 0);
  free_struct(res);
}
END_TEST

START_TEST(affine1) {
  data *res = parser("./tests/test.test");
  ck_assert_int_eq(res->correct, 0);
  center(res->matrix);
  normalize_size(res->matrix);
  transmit(1, -1, 3.5, res->matrix);
  rotate_x(12, res->matrix);
  rotate_y(2.36, res->matrix);
  rotate_z(-1.22, res->matrix);
  free_struct(res);
}
END_TEST

Suite *suite_parser() {
  Suite *s = suite_create("parser");
  TCase *temp = tcase_create("parser");

  suite_add_tcase(s, temp);

  tcase_add_test(temp, parser1);
  tcase_add_test(temp, affine1);
  return s;
}