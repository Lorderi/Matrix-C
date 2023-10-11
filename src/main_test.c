#include <check.h>
#include <locale.h>
#include <stdlib.h>

#include "matrix.h"

#define TEST_EPS 1e-06

START_TEST(create_test_1) {
  matrix_t M;
  int status = create_matrix(0, 0, &M);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(create_test_2) {
  matrix_t M;
  int status = create_matrix(5, 0, &M);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(create_test_3) {
  matrix_t M;
  int status = create_matrix(0, 5, &M);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(create_test_4) {
  matrix_t M;
  int status = create_matrix(-3, 0, &M);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(create_test_5) {
  matrix_t M;
  int status = create_matrix(-3, 5, &M);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(create_test_6) {
  matrix_t M;
  int status = create_matrix(5, -3, &M);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(create_test_7) {
  matrix_t M;
  int status = create_matrix(0, -3, &M);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(create_test_9) {
  matrix_t M;
  int status = create_matrix(3, 3, &M);
  remove_matrix(&M);
  ck_assert_int_eq(status, OK);
}
END_TEST

START_TEST(remove_test) {
  matrix_t M;
  int rows = 0;
  int columns = 7;
  int status = create_matrix(rows, columns, &M);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(equal_test_2) {
  matrix_t M1;
  matrix_t M2;
  int rows = 7;
  int columns = 10;
  int status1 = create_matrix(rows, columns, &M1);
  int status2 = create_matrix(rows, columns, &M2);
  ck_assert_int_eq(status1, 0);
  ck_assert_int_eq(status2, 0);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++) {
      M1.matrix[i][j] = (double)(i + j) + 0.123;
      M2.matrix[i][j] = M1.matrix[i][j] + 0.00000001;
    }
  int status_eq = eq_matrix(&M1, &M2);
  ck_assert_int_eq(status_eq, 1);
  remove_matrix(&M1);
  remove_matrix(&M2);
}
END_TEST

START_TEST(create_test_8) {
  matrix_t *matrix = malloc(sizeof(matrix_t));
  ck_assert_uint_eq(1, create_matrix(-50, 100, matrix));

  ck_assert_uint_eq(OK, create_matrix(2, 3, matrix));
  remove_matrix(matrix);

  ck_assert_uint_eq(OK, create_matrix(5, 5, matrix));

  ck_assert_uint_eq(ERR_MATR, create_matrix(2, 2, NULL));

  double test = -111.1611;
  matrix->matrix[2][2] = test;
  ck_assert_uint_eq(5, matrix->columns);
  ck_assert_uint_eq(5, matrix->rows);
  ck_assert_double_eq(matrix->matrix[2][2], test);
  remove_matrix(matrix);
  free(matrix);
}
END_TEST

START_TEST(equal_test_1) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *matrix_2 = malloc(sizeof(matrix_t));

  create_matrix(2, 3, matrix_1);
  create_matrix(2, 2, matrix_2);
  int output = eq_matrix(matrix_1, matrix_2);
  ck_assert_uint_eq(FAILURE, output);

  output = eq_matrix(matrix_1, NULL);
  ck_assert_uint_eq(FAILURE, output);

  remove_matrix(matrix_1);
  remove_matrix(matrix_2);
  create_matrix(2, 2, matrix_1);
  create_matrix(2, 2, matrix_2);
  output = eq_matrix(matrix_1, matrix_2);
  ck_assert_uint_eq(SUCCESS, output);

  remove_matrix(matrix_1);
  remove_matrix(matrix_2);
  create_matrix(2, 2, matrix_1);
  create_matrix(2, 2, matrix_2);
  matrix_1->matrix[0][0] = 1.098;
  matrix_2->matrix[0][0] = 1.098;
  matrix_1->matrix[0][1] = 123.098;
  matrix_2->matrix[0][1] = 123.098;
  matrix_1->matrix[1][0] = -56123.098;
  matrix_2->matrix[1][0] = -56123.098;
  matrix_1->matrix[1][1] = 123.04498;
  matrix_2->matrix[1][1] = 123.04498;
  output = eq_matrix(matrix_1, matrix_2);
  ck_assert_uint_eq(SUCCESS, output);

  remove_matrix(matrix_1);
  remove_matrix(matrix_2);
  create_matrix(20, 20, matrix_1);
  create_matrix(20, 20, matrix_2);
  for (int i = 0; i < matrix_1->rows; i++) {
    for (int j = 0; j < matrix_1->columns; j++) {
      matrix_1->matrix[i][j] = i + j * 0.00001;
      matrix_2->matrix[i][j] = i + j * 0.00002;
    }
  }
  output = eq_matrix(matrix_1, matrix_2);
  ck_assert_uint_eq(FAILURE, output);

  remove_matrix(matrix_1);
  remove_matrix(matrix_2);
  create_matrix(200, 200, matrix_1);
  create_matrix(200, 200, matrix_2);
  for (int i = 0; i < matrix_1->rows; i++) {
    for (int j = 0; j < matrix_1->columns; j++) {
      matrix_1->matrix[i][j] = i + j * 0.00001;
      matrix_2->matrix[i][j] = i + j * 0.00001;
    }
  }
  output = eq_matrix(matrix_1, matrix_2);
  ck_assert_uint_eq(SUCCESS, output);

  remove_matrix(matrix_1);
  remove_matrix(matrix_2);
  free(matrix_1);
  free(matrix_2);
}
END_TEST

START_TEST(sum_test_1) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *matrix_2 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  matrix_t *real_result = malloc(sizeof(matrix_t));

  create_matrix(20, 20, matrix_1);
  create_matrix(20, 20, matrix_2);
  create_matrix(20, 20, real_result);
  for (int i = 0; i < matrix_1->rows; i++) {
    for (int j = 0; j < matrix_1->columns; j++) {
      matrix_1->matrix[i][j] = (i + j) * 0.00001;
      matrix_2->matrix[i][j] = (i + j) * 0.00001;
      real_result->matrix[i][j] = (i + j) * 0.00002;
    }
  }
  int output = sum_matrix(matrix_1, NULL, result);
  ck_assert_uint_eq(ERR_MATR, output);

  output = sum_matrix(matrix_1, matrix_2, NULL);
  ck_assert_uint_eq(ERR_MATR, output);

  output = sum_matrix(matrix_1, matrix_2, result);
  ck_assert_uint_eq(OK, output);
  for (int i = 0; i < matrix_1->rows; i++) {
    for (int j = 0; j < matrix_1->columns; j++)
      ck_assert_double_eq(real_result->matrix[i][j], result->matrix[i][j]);
  }
  remove_matrix(matrix_1);
  remove_matrix(matrix_2);
  remove_matrix(result);
  remove_matrix(real_result);

  free(matrix_1);
  free(matrix_2);
  free(result);
  free(real_result);
}
END_TEST

START_TEST(sum_test_2) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *matrix_2 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  result->matrix = NULL;
  create_matrix(20, 20, matrix_1);
  create_matrix(20, 20, matrix_2);
  remove_matrix(matrix_2);

  for (int i = 0; i < matrix_1->rows; i++) {
    for (int j = 0; j < matrix_1->columns; j++)
      matrix_1->matrix[i][j] = (i + j) * 0.00001;
  }
  int output = sum_matrix(matrix_1, matrix_2, result);
  ck_assert_uint_eq(ERR_MATR, output);

  remove_matrix(matrix_1);

  remove_matrix(result);

  free(matrix_1);
  free(matrix_2);
  free(result);
}
END_TEST

START_TEST(sub_test_1) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *matrix_2 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  matrix_t *real_result = malloc(sizeof(matrix_t));
  create_matrix(20, 20, matrix_1);
  create_matrix(20, 20, matrix_2);
  create_matrix(20, 20, real_result);

  for (int i = 0; i < matrix_1->rows; i++) {
    for (int j = 0; j < matrix_1->columns; j++) {
      matrix_1->matrix[i][j] = (i + j) * 0.00001;
      matrix_2->matrix[i][j] = (i + j) * 0.00001;
      real_result->matrix[i][j] =
          matrix_1->matrix[i][j] - matrix_2->matrix[i][j];
    }
  }
  int output = sub_matrix(matrix_1, matrix_2, result);
  ck_assert_uint_eq(OK, output);
  for (int i = 0; i < matrix_1->rows; i++) {
    for (int j = 0; j < matrix_1->columns; j++)
      ck_assert_double_eq(real_result->matrix[i][j], result->matrix[i][j]);
  }

  remove_matrix(matrix_1);
  remove_matrix(matrix_2);
  remove_matrix(result);
  remove_matrix(real_result);
  free(matrix_1);
  free(matrix_2);
  free(result);
  free(real_result);
}
END_TEST

START_TEST(sub_test_2) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *matrix_2 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  result->matrix = NULL;
  create_matrix(20, 20, matrix_1);
  create_matrix(30, 20, matrix_2);

  int output = sub_matrix(matrix_1, matrix_2, result);
  ck_assert_uint_eq(ERR_CALC, output);

  remove_matrix(matrix_1);
  remove_matrix(matrix_2);
  remove_matrix(result);
  free(matrix_1);
  free(matrix_2);
  free(result);
}
END_TEST

START_TEST(mul_number_test_1) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  result->matrix = NULL;
  double number = 56.073;
  create_matrix(20, 20, matrix_1);
  remove_matrix(matrix_1);

  int output = mult_number(matrix_1, number, result);
  ck_assert_uint_eq(ERR_MATR, output);

  output = mult_number(NULL, number, result);
  ck_assert_uint_eq(ERR_MATR, output);

  output = mult_number(matrix_1, number, NULL);
  ck_assert_uint_eq(ERR_MATR, output);

  remove_matrix(result);
  free(matrix_1);
  free(result);
}
END_TEST

START_TEST(mul_number_test_2) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *real_result = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  double number = 56.073;
  create_matrix(20, 20, matrix_1);
  create_matrix(20, 20, real_result);

  for (int i = 0; i < matrix_1->rows; i++) {
    for (int j = 0; j < matrix_1->columns; j++) {
      matrix_1->matrix[i][j] = (i + j) * 0.00001;
      real_result->matrix[i][j] = matrix_1->matrix[i][j] * number;
    }
  }

  int output = mult_number(matrix_1, number, result);
  ck_assert_uint_eq(OK, output);
  for (int i = 0; i < matrix_1->rows; i++) {
    for (int j = 0; j < matrix_1->columns; j++)
      ck_assert_double_eq(real_result->matrix[i][j], result->matrix[i][j]);
  }

  remove_matrix(matrix_1);
  remove_matrix(result);
  remove_matrix(real_result);
  free(matrix_1);
  free(result);
  free(real_result);
}
END_TEST

START_TEST(mul_matrix_test_1) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *matrix_2 = malloc(sizeof(matrix_t));
  matrix_t *matrix_3 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  result->matrix = NULL;
  create_matrix(20, 20, matrix_1);
  create_matrix(20, 20, matrix_2);
  create_matrix(20, 21, matrix_3);
  remove_matrix(matrix_1);

  int output = mult_matrix(matrix_1, matrix_2, result);
  ck_assert_uint_eq(ERR_MATR, output);

  output = mult_matrix(NULL, matrix_2, result);
  ck_assert_uint_eq(ERR_MATR, output);

  output = mult_matrix(matrix_1, matrix_2, NULL);
  ck_assert_uint_eq(ERR_MATR, output);

  output = mult_matrix(matrix_3, matrix_2, result);
  ck_assert_uint_eq(ERR_CALC, output);

  remove_matrix(matrix_2);
  remove_matrix(matrix_3);
  remove_matrix(result);
  free(matrix_1);
  free(matrix_2);
  free(matrix_3);
  free(result);
}
END_TEST

START_TEST(mul_matrix_test_2) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *matrix_2 = malloc(sizeof(matrix_t));
  matrix_t *real_result = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  create_matrix(2, 2, matrix_1);
  create_matrix(2, 2, matrix_2);
  create_matrix(2, 2, real_result);

  matrix_1->matrix[0][0] = 1;
  matrix_1->matrix[0][1] = 2;
  matrix_1->matrix[1][0] = 3;
  matrix_1->matrix[1][1] = 4;

  matrix_2->matrix[0][0] = 3;
  matrix_2->matrix[0][1] = 4;
  matrix_2->matrix[1][0] = 5;
  matrix_2->matrix[1][1] = 6;

  real_result->matrix[0][0] = 13;
  real_result->matrix[0][1] = 16;
  real_result->matrix[1][0] = 29;
  real_result->matrix[1][1] = 36;

  int output = mult_matrix(matrix_1, matrix_2, result);
  ck_assert_uint_eq(OK, output);
  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++)
      ck_assert_double_eq(real_result->matrix[i][j], result->matrix[i][j]);
  }

  remove_matrix(matrix_1);
  remove_matrix(matrix_2);
  remove_matrix(result);
  remove_matrix(real_result);
  free(matrix_1);
  free(matrix_2);
  free(result);
  free(real_result);
}
END_TEST

START_TEST(mul_matrix_test_3) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *matrix_2 = malloc(sizeof(matrix_t));
  matrix_t *real_result_1 = malloc(sizeof(matrix_t));
  matrix_t *real_result_2 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  create_matrix(2, 3, matrix_1);
  create_matrix(3, 2, matrix_2);
  create_matrix(3, 3, real_result_1);
  create_matrix(3, 3, real_result_2);

  matrix_1->matrix[0][0] = 3.9;
  matrix_1->matrix[0][1] = 0.4;
  matrix_1->matrix[0][2] = 8345;
  matrix_1->matrix[1][0] = 5.8354;
  matrix_1->matrix[1][1] = 6.0;
  matrix_1->matrix[1][2] = 24.9;

  matrix_2->matrix[0][0] = 138.49;
  matrix_2->matrix[0][1] = 245.6;
  matrix_2->matrix[1][0] = 3.0;
  matrix_2->matrix[1][1] = 0.844;
  matrix_2->matrix[2][0] = 0;
  matrix_2->matrix[2][1] = 0;

  real_result_1->matrix[0][0] = 541.311;
  real_result_1->matrix[0][1] = 958.1776;
  real_result_1->matrix[1][0] = 826.144546;
  real_result_1->matrix[1][1] = 1438.23824;

  real_result_2->matrix[0][0] = 1973.28524;
  real_result_2->matrix[0][1] = 1528.996;
  real_result_2->matrix[0][2] = 1161814.49;
  real_result_2->matrix[1][0] = 16.6250776;
  real_result_2->matrix[1][1] = 6.264;
  real_result_2->matrix[1][2] = 25056.0156;
  real_result_2->matrix[2][0] = 0;
  real_result_2->matrix[2][1] = 0;
  real_result_2->matrix[2][2] = 0;

  int output = mult_matrix(matrix_2, matrix_1, result);
  ck_assert_uint_eq(OK, output);
  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++)
      ck_assert_double_eq_tol(real_result_2->matrix[i][j], result->matrix[i][j],
                              TEST_EPS);
  }

  remove_matrix(result);
  output = mult_matrix(matrix_1, matrix_2, result);
  ck_assert_uint_eq(OK, output);
  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++)
      ck_assert_double_eq_tol(real_result_1->matrix[i][j], result->matrix[i][j],
                              TEST_EPS);
  }

  remove_matrix(matrix_1);
  remove_matrix(matrix_2);
  remove_matrix(result);
  remove_matrix(real_result_1);
  remove_matrix(real_result_2);
  free(matrix_1);
  free(matrix_2);
  free(result);
  free(real_result_1);
  free(real_result_2);
}
END_TEST

START_TEST(mul_matrix_test_4) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *matrix_2 = malloc(sizeof(matrix_t));
  matrix_t *real_result_1 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  create_matrix(1, 3, matrix_1);
  create_matrix(3, 2, matrix_2);
  create_matrix(3, 3, real_result_1);

  matrix_1->matrix[0][0] = 1.5;
  matrix_1->matrix[0][1] = 2;
  matrix_1->matrix[0][2] = 3;

  matrix_2->matrix[0][0] = 1;
  matrix_2->matrix[0][1] = 12;
  matrix_2->matrix[1][0] = 2;
  matrix_2->matrix[1][1] = 231;
  matrix_2->matrix[2][0] = 3;
  matrix_2->matrix[2][1] = 21;

  real_result_1->matrix[0][0] = 14.5;
  real_result_1->matrix[0][1] = 543.0;

  int output = mult_matrix(matrix_1, matrix_2, result);
  ck_assert_uint_eq(OK, output);
  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++)
      ck_assert_double_eq_tol(real_result_1->matrix[i][j], result->matrix[i][j],
                              TEST_EPS);
  }

  remove_matrix(matrix_1);
  remove_matrix(matrix_2);
  remove_matrix(result);
  remove_matrix(real_result_1);
  free(matrix_1);
  free(matrix_2);
  free(result);
  free(real_result_1);
}
END_TEST

START_TEST(transpose_test_1) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *real_result = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  result->matrix = NULL;
  create_matrix(2, 3, matrix_1);
  create_matrix(3, 3, real_result);

  matrix_1->matrix[0][0] = 3.9;
  matrix_1->matrix[0][1] = 0.4;
  matrix_1->matrix[0][2] = 8345;
  matrix_1->matrix[1][0] = 5.8354;
  matrix_1->matrix[1][1] = 6.0;
  matrix_1->matrix[1][2] = 24.9;

  real_result->matrix[0][0] = 3.9;
  real_result->matrix[0][1] = 5.8354;
  real_result->matrix[1][0] = 0.4;
  real_result->matrix[1][1] = 6.0;
  real_result->matrix[2][0] = 8345;
  real_result->matrix[2][1] = 24.9;

  int output = transpose(NULL, result);
  ck_assert_uint_eq(ERR_MATR, output);

  output = transpose(matrix_1, NULL);
  ck_assert_uint_eq(ERR_MATR, output);

  output = transpose(matrix_1, result);
  ck_assert_uint_eq(OK, output);
  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++)
      ck_assert_double_eq(real_result->matrix[i][j], result->matrix[i][j]);
  }

  remove_matrix(matrix_1);
  remove_matrix(result);
  remove_matrix(real_result);
  free(matrix_1);
  free(result);
  free(real_result);
}
END_TEST

START_TEST(transpose_test_2) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  result->matrix = NULL;
  create_matrix(20, 20, matrix_1);
  remove_matrix(matrix_1);

  int output = transpose(matrix_1, result);
  ck_assert_uint_eq(ERR_MATR, output);

  remove_matrix(result);
  free(matrix_1);
  free(result);
}
END_TEST

START_TEST(determinant_test_1) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *matrix_2 = malloc(sizeof(matrix_t));
  double result;
  create_matrix(20, 20, matrix_1);
  create_matrix(20, 21, matrix_2);
  remove_matrix(matrix_1);

  int output = determinant(matrix_1, &result);
  ck_assert_uint_eq(ERR_MATR, output);

  output = determinant(NULL, &result);
  ck_assert_uint_eq(ERR_MATR, output);

  output = determinant(matrix_1, NULL);
  ck_assert_uint_eq(ERR_MATR, output);

  output = determinant(matrix_2, &result);
  ck_assert_uint_eq(ERR_CALC, output);

  remove_matrix(matrix_2);
  free(matrix_1);
  free(matrix_2);
}
END_TEST

START_TEST(determinant_test_2) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  double result;
  create_matrix(2, 2, matrix_1);

  matrix_1->matrix[0][0] = 109.56;
  matrix_1->matrix[0][1] = 25;
  matrix_1->matrix[1][0] = 334.684;
  matrix_1->matrix[1][1] = 0.0;

  int output = determinant(matrix_1, &result);
  ck_assert_uint_eq(OK, output);
  ck_assert_double_eq(-8367.1, result);

  remove_matrix(matrix_1);
  free(matrix_1);
}
END_TEST

START_TEST(determinant_test_3) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  double result = 0;
  create_matrix(3, 3, matrix_1);

  matrix_1->matrix[0][0] = 109.56;
  matrix_1->matrix[0][1] = 25;
  matrix_1->matrix[0][2] = 334.684;
  matrix_1->matrix[1][0] = 0.0;
  matrix_1->matrix[1][1] = 0;
  matrix_1->matrix[1][2] = 34.86;
  matrix_1->matrix[2][0] = 7;
  matrix_1->matrix[2][1] = 8;
  matrix_1->matrix[2][2] = 9.0;

  int output = determinant(matrix_1, &result);
  ck_assert_uint_eq(OK, output);
  ck_assert_double_eq(-24453.5928, result);

  remove_matrix(matrix_1);
  free(matrix_1);
}
END_TEST

START_TEST(determinant_test_4) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  double result;
  create_matrix(1, 1, matrix_1);

  matrix_1->matrix[0][0] = 109.56;

  int output = determinant(matrix_1, &result);
  ck_assert_uint_eq(OK, output);
  ck_assert_double_eq(109.56, result);

  remove_matrix(matrix_1);
  free(matrix_1);
}
END_TEST

START_TEST(determinant_test_5) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  double result = 0;
  create_matrix(4, 4, matrix_1);

  matrix_1->matrix[0][0] = 9;
  matrix_1->matrix[0][1] = 7;
  matrix_1->matrix[0][2] = 0;
  matrix_1->matrix[0][3] = 0;
  matrix_1->matrix[1][0] = 7.3;
  matrix_1->matrix[1][1] = 0.0;
  matrix_1->matrix[1][2] = 0;
  matrix_1->matrix[1][3] = 0;
  matrix_1->matrix[2][0] = 0;
  matrix_1->matrix[2][1] = 0;
  matrix_1->matrix[2][2] = 5;
  matrix_1->matrix[2][3] = 0;
  matrix_1->matrix[3][0] = 0;
  matrix_1->matrix[3][1] = 4;
  matrix_1->matrix[3][2] = 0;
  matrix_1->matrix[3][3] = 10;

  int output = determinant(matrix_1, &result);
  ck_assert_uint_eq(OK, output);
  ck_assert_double_eq(-2555, result);

  remove_matrix(matrix_1);
  free(matrix_1);
}
END_TEST

START_TEST(complements_test_1) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  result->matrix = NULL;
  create_matrix(3, 3, matrix_1);
  remove_matrix(matrix_1);

  int output = calc_complements(matrix_1, result);
  ck_assert_uint_eq(ERR_MATR, output);

  output = calc_complements(NULL, result);
  ck_assert_uint_eq(ERR_MATR, output);

  output = calc_complements(matrix_1, NULL);
  ck_assert_uint_eq(ERR_MATR, output);

  remove_matrix(result);
  free(matrix_1);
  free(result);
}
END_TEST

START_TEST(complements_test_2) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  result->matrix = NULL;
  matrix_t *real_result = malloc(sizeof(matrix_t));

  create_matrix(3, 3, matrix_1);
  create_matrix(3, 3, real_result);

  matrix_1->matrix[0][0] = 1;
  matrix_1->matrix[0][1] = 2;
  matrix_1->matrix[0][2] = 3;
  matrix_1->matrix[1][0] = 0;
  matrix_1->matrix[1][1] = 4;
  matrix_1->matrix[1][2] = 2;
  matrix_1->matrix[2][0] = 5;
  matrix_1->matrix[2][1] = 2;
  matrix_1->matrix[2][2] = 1;

  int output = calc_complements(matrix_1, result);
  ck_assert_uint_eq(OK, output);

  real_result->matrix[0][0] = 0;
  real_result->matrix[0][1] = 10;
  real_result->matrix[0][2] = -20;
  real_result->matrix[1][0] = 4;
  real_result->matrix[1][1] = -14;
  real_result->matrix[1][2] = 8;
  real_result->matrix[2][0] = -8;
  real_result->matrix[2][1] = -2;
  real_result->matrix[2][2] = 4;

  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++)
      ck_assert_double_eq(real_result->matrix[i][j], result->matrix[i][j]);
  }

  remove_matrix(matrix_1);
  remove_matrix(result);
  remove_matrix(real_result);
  free(matrix_1);
  free(result);
  free(real_result);
}
END_TEST

START_TEST(complements_test_3) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  result->matrix = NULL;
  create_matrix(1, 1, matrix_1);

  matrix_1->matrix[0][0] = 1;

  int output = calc_complements(matrix_1, result);
  ck_assert_uint_eq(OK, output);
  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++)
      ck_assert_double_eq_tol(1, result->matrix[i][j], TEST_EPS);
  }

  remove_matrix(matrix_1);
  remove_matrix(result);
  free(matrix_1);
  free(result);
}
END_TEST

START_TEST(complements_test_4) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  result->matrix = NULL;
  matrix_t *real_result = malloc(sizeof(matrix_t));
  create_matrix(4, 4, matrix_1);
  create_matrix(4, 4, real_result);

  matrix_1->matrix[0][0] = 9;
  matrix_1->matrix[0][1] = 7;
  matrix_1->matrix[0][2] = 0;
  matrix_1->matrix[0][3] = 0;
  matrix_1->matrix[1][0] = 7.3;
  matrix_1->matrix[1][1] = 0.0;
  matrix_1->matrix[1][2] = 0;
  matrix_1->matrix[1][3] = 0;
  matrix_1->matrix[2][0] = 0;
  matrix_1->matrix[2][1] = 0;
  matrix_1->matrix[2][2] = 5;
  matrix_1->matrix[2][3] = 0;
  matrix_1->matrix[3][0] = 0;
  matrix_1->matrix[3][1] = 4;
  matrix_1->matrix[3][2] = 0;
  matrix_1->matrix[3][3] = 10;

  int output = calc_complements(matrix_1, result);
  ck_assert_uint_eq(OK, output);

  real_result->matrix[0][0] = 0;
  real_result->matrix[0][1] = -365;
  real_result->matrix[0][2] = 0;
  real_result->matrix[0][3] = 146;
  real_result->matrix[1][0] = -350;
  real_result->matrix[1][1] = 450;
  real_result->matrix[1][2] = 0;
  real_result->matrix[1][3] = -180;
  real_result->matrix[2][0] = 0;
  real_result->matrix[2][1] = 0;
  real_result->matrix[2][2] = -511;
  real_result->matrix[2][3] = 0;
  real_result->matrix[3][0] = 0;
  real_result->matrix[3][1] = 0;
  real_result->matrix[3][2] = 0;
  real_result->matrix[3][3] = -255.5;

  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++)
      ck_assert_double_eq_tol(real_result->matrix[i][j], result->matrix[i][j],
                              TEST_EPS);
  }

  remove_matrix(matrix_1);
  remove_matrix(result);
  remove_matrix(real_result);
  free(matrix_1);
  free(result);
  free(real_result);
}
END_TEST

START_TEST(complements_test_5) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  result->matrix = NULL;
  create_matrix(1, 2, matrix_1);

  matrix_1->matrix[0][0] = 1;
  matrix_1->matrix[0][1] = 1;

  int output = calc_complements(matrix_1, result);
  ck_assert_uint_eq(ERR_CALC, output);

  remove_matrix(matrix_1);
  remove_matrix(result);
  free(matrix_1);
  free(result);
}
END_TEST

START_TEST(inverse_test_1) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *matrix_2 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  result->matrix = NULL;
  create_matrix(20, 20, matrix_1);
  create_matrix(20, 21, matrix_2);
  remove_matrix(matrix_1);

  int output = inverse_matrix(matrix_1, result);
  ck_assert_uint_eq(ERR_MATR, output);

  output = inverse_matrix(NULL, result);
  ck_assert_uint_eq(ERR_MATR, output);

  output = inverse_matrix(matrix_1, NULL);
  ck_assert_uint_eq(ERR_MATR, output);

  output = inverse_matrix(matrix_2, result);
  ck_assert_uint_eq(ERR_CALC, output);

  remove_matrix(matrix_2);
  remove_matrix(result);
  free(matrix_1);
  free(matrix_2);
  free(result);
}
END_TEST

START_TEST(inverse_test_2) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  result->matrix = NULL;
  create_matrix(2, 2, matrix_1);

  matrix_1->matrix[0][0] = 1;
  matrix_1->matrix[0][1] = 1;
  matrix_1->matrix[1][0] = 1;
  matrix_1->matrix[1][1] = 1;

  int output = inverse_matrix(matrix_1, result);
  ck_assert_uint_eq(ERR_CALC, output);

  remove_matrix(matrix_1);
  remove_matrix(result);
  free(matrix_1);
  free(result);
}
END_TEST

START_TEST(inverse_test_3) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  create_matrix(1, 1, matrix_1);

  matrix_1->matrix[0][0] = 1;

  int output = inverse_matrix(matrix_1, result);
  ck_assert_uint_eq(OK, output);

  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++)
      ck_assert_double_eq(1, result->matrix[i][j]);
  }

  remove_matrix(matrix_1);
  remove_matrix(result);
  free(matrix_1);
  free(result);
}
END_TEST

START_TEST(inverse_test_4) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  create_matrix(1, 1, matrix_1);

  matrix_1->matrix[0][0] = 2;

  int output = inverse_matrix(matrix_1, result);
  ck_assert_uint_eq(OK, output);

  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++)
      ck_assert_double_eq(0.5, result->matrix[i][j]);
  }

  remove_matrix(matrix_1);
  remove_matrix(result);
  free(matrix_1);
  free(result);
}
END_TEST

START_TEST(inverse_test_5) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  matrix_t *real_result = malloc(sizeof(matrix_t));
  create_matrix(2, 2, matrix_1);
  create_matrix(2, 2, real_result);

  matrix_1->matrix[0][0] = 1;
  matrix_1->matrix[0][1] = 2;
  matrix_1->matrix[1][0] = 3;
  matrix_1->matrix[1][1] = 5;

  int output = inverse_matrix(matrix_1, result);
  ck_assert_uint_eq(OK, output);

  real_result->matrix[0][0] = -5;
  real_result->matrix[0][1] = 2;
  real_result->matrix[1][0] = 3;
  real_result->matrix[1][1] = -1;

  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++)
      ck_assert_double_eq(real_result->matrix[i][j], result->matrix[i][j]);
  }

  remove_matrix(matrix_1);
  remove_matrix(result);
  remove_matrix(real_result);
  free(matrix_1);
  free(result);
  free(real_result);
}
END_TEST

START_TEST(inverse_test_6) {
  matrix_t *matrix_1 = malloc(sizeof(matrix_t));
  matrix_t *result = malloc(sizeof(matrix_t));
  matrix_t *real_result = malloc(sizeof(matrix_t));
  create_matrix(3, 3, matrix_1);
  create_matrix(3, 3, real_result);

  matrix_1->matrix[0][0] = 1;
  matrix_1->matrix[0][1] = 2;
  matrix_1->matrix[0][2] = 2;
  matrix_1->matrix[1][0] = 3;
  matrix_1->matrix[1][1] = 5;
  matrix_1->matrix[1][2] = 5;
  matrix_1->matrix[2][0] = 3;
  matrix_1->matrix[2][1] = 5;
  matrix_1->matrix[2][2] = 6;

  int output = inverse_matrix(matrix_1, result);
  ck_assert_uint_eq(OK, output);

  real_result->matrix[0][0] = -5;
  real_result->matrix[0][1] = 2;
  real_result->matrix[0][2] = 0;
  real_result->matrix[1][0] = 3;
  real_result->matrix[1][1] = 0;
  real_result->matrix[1][2] = -1;
  real_result->matrix[2][0] = 0;
  real_result->matrix[2][1] = -1;
  real_result->matrix[2][2] = 1;

  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++)
      ck_assert_double_eq(real_result->matrix[i][j], result->matrix[i][j]);
  }

  remove_matrix(matrix_1);
  remove_matrix(result);
  remove_matrix(real_result);
  free(matrix_1);
  free(result);
  free(real_result);
}
END_TEST

START_TEST(inverse_matrix_loop_test) {
  matrix_t start;
  matrix_t result_matrix;
  matrix_t result_matrix2;
  create_matrix(3, 3, &start);
  start.matrix[0][0] = rand();
  start.matrix[0][1] = rand();
  start.matrix[0][2] = rand();
  start.matrix[1][0] = rand();
  start.matrix[1][1] = rand();
  start.matrix[1][2] = rand();
  start.matrix[2][0] = rand();
  start.matrix[2][1] = rand();
  start.matrix[2][2] = rand();
  ck_assert_uint_eq(OK, inverse_matrix(&start, &result_matrix2));
  ck_assert_uint_eq(OK,
                    mult_matrix(&start, &result_matrix2, &result_matrix));

  ck_assert_double_eq_tol(result_matrix.matrix[0][0], 1, 1e-8);
  ck_assert_double_eq_tol(result_matrix.matrix[0][1], 0, 1e-8);
  ck_assert_double_eq_tol(result_matrix.matrix[0][2], 0, 1e-8);
  ck_assert_double_eq_tol(result_matrix.matrix[1][0], 0, 1e-8);
  ck_assert_double_eq_tol(result_matrix.matrix[1][1], 1, 1e-8);
  ck_assert_double_eq_tol(result_matrix.matrix[1][2], 0, 1e-8);
  ck_assert_double_eq_tol(result_matrix.matrix[2][0], 0, 1e-8);
  ck_assert_double_eq_tol(result_matrix.matrix[2][1], 0, 1e-8);
  ck_assert_double_eq_tol(result_matrix.matrix[2][2], 1, 1e-8);

  remove_matrix(&start);
  remove_matrix(&result_matrix);
  remove_matrix(&result_matrix2);
}
END_TEST

int main(void) {
  Suite *s = suite_create("matrix");
  SRunner *sr = srunner_create(s);
  int number_failed;

  TCase *tc_matrix = tcase_create("matrix");
  suite_add_tcase(s, tc_matrix);
  tcase_add_test(tc_matrix, create_test_1);
  tcase_add_test(tc_matrix, create_test_2);
  tcase_add_test(tc_matrix, create_test_3);
  tcase_add_test(tc_matrix, create_test_4);
  tcase_add_test(tc_matrix, create_test_5);
  tcase_add_test(tc_matrix, create_test_6);
  tcase_add_test(tc_matrix, create_test_7);
  tcase_add_test(tc_matrix, create_test_8);
  tcase_add_test(tc_matrix, create_test_9);
  tcase_add_test(tc_matrix, remove_test);
  tcase_add_test(tc_matrix, equal_test_1);
  tcase_add_test(tc_matrix, equal_test_2);
  tcase_add_test(tc_matrix, sum_test_1);
  tcase_add_test(tc_matrix, sum_test_2);
  tcase_add_test(tc_matrix, sub_test_1);
  tcase_add_test(tc_matrix, sub_test_2);
  tcase_add_test(tc_matrix, mul_number_test_1);
  tcase_add_test(tc_matrix, mul_number_test_2);
  tcase_add_test(tc_matrix, mul_matrix_test_1);
  tcase_add_test(tc_matrix, mul_matrix_test_2);
  tcase_add_test(tc_matrix, mul_matrix_test_3);
  tcase_add_test(tc_matrix, mul_matrix_test_4);
  tcase_add_test(tc_matrix, transpose_test_1);
  tcase_add_test(tc_matrix, transpose_test_2);
  tcase_add_test(tc_matrix, determinant_test_1);
  tcase_add_test(tc_matrix, determinant_test_2);
  tcase_add_test(tc_matrix, determinant_test_3);
  tcase_add_test(tc_matrix, determinant_test_4);
  tcase_add_test(tc_matrix, determinant_test_5);
  tcase_add_test(tc_matrix, complements_test_1);
  tcase_add_test(tc_matrix, complements_test_2);
  tcase_add_test(tc_matrix, complements_test_3);
  tcase_add_test(tc_matrix, complements_test_4);
  tcase_add_test(tc_matrix, complements_test_5);
  tcase_add_test(tc_matrix, inverse_test_1);
  tcase_add_test(tc_matrix, inverse_test_2);
  tcase_add_test(tc_matrix, inverse_test_3);
  tcase_add_test(tc_matrix, inverse_test_4);
  tcase_add_test(tc_matrix, inverse_test_5);
  tcase_add_test(tc_matrix, inverse_test_6);
  tcase_add_loop_test(tc_matrix, inverse_matrix_loop_test, 0, 10);

  srunner_run_all(sr, CK_ENV);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : -1;
}
