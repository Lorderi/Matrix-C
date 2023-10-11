#include "matrix.h"

int create_matrix(int rows, int columns, matrix_t *result) {
  mreturn_t rtrn_value = ERR_MATR;
  if (result && rows > 0 && columns > 0) {
    result->rows = 0;
    result->columns = 0;
    result->matrix = (double **)malloc(rows * sizeof(double *) +
                                       rows * columns * sizeof(double));
    if (result->matrix) {
      result->rows = rows;
      result->columns = columns;
      double *ptr = (double *)(result->matrix + rows);
      for (int i = 0; i < rows; i++) result->matrix[i] = ptr + columns * i;
      memset(ptr, 0, rows * columns * sizeof(double));
      rtrn_value = OK;
    }
  }
  return rtrn_value;
}

void remove_matrix(matrix_t *A) {
  if (A) {
    if (A->matrix) free(A->matrix);
    A->rows = 0;
    A->columns = 0;
  }
}

int eq_matrix(matrix_t *A, matrix_t *B) {
  meq_t equal_value = FAILURE;
  if (mtrx_normal(A) && mtrx_normal(B) &&
      mtrx_equal_borders(A, B)) {
    equal_value = SUCCESS;
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; equal_value && j < A->columns; j++)
        if (over_precision(A->matrix[i][j] - B->matrix[i][j]))
          equal_value = FAILURE;
  }
  return equal_value;
}

int simple_operation(matrix_t *A, matrix_t *B, matrix_t *result,
                          double func(double, double)) {
  mreturn_t rtrn_value = OK;
  if (!result || !mtrx_normal(A) || !mtrx_normal(B))
    rtrn_value = ERR_MATR;
  else if (!mtrx_equal_borders(A, B) ||
           create_matrix(A->rows, A->columns, result))
    rtrn_value = ERR_CALC;
  else {
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = func(A->matrix[i][j], B->matrix[i][j]);
  }
  return rtrn_value;
}

int sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  mreturn_t rtrn_value = simple_operation(A, B, result, addition);
  return rtrn_value;
}

int sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  mreturn_t rtrn_value = simple_operation(A, B, result, subtraction);
  return rtrn_value;
}

int mult_number(matrix_t *A, double number, matrix_t *result) {
  mreturn_t rtrn_value = OK;
  if (!result || !mtrx_normal(A))
    rtrn_value = ERR_MATR;
  else if (create_matrix(A->rows, A->columns, result))
    rtrn_value = ERR_CALC;
  else {
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = A->matrix[i][j] * number;
  }
  return rtrn_value;
}

int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  mreturn_t rtrn_value = OK;
  if (!result || !mtrx_normal(A) || !mtrx_normal(B))
    rtrn_value = ERR_MATR;
  else if (A->columns != B->rows ||
           create_matrix(A->rows, B->columns, result))
    rtrn_value = ERR_CALC;
  else {
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < B->columns; j++)
        for (int k = 0; k < A->columns; k++)
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
  }
  return rtrn_value;
}

int transpose(matrix_t *A, matrix_t *result) {
  mreturn_t rtrn_value = OK;
  if (!result || !mtrx_normal(A))
    rtrn_value = ERR_MATR;
  else if (create_matrix(A->columns, A->rows, result)) {
    rtrn_value = ERR_CALC;
  } else {
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[j][i] = A->matrix[i][j];
  }
  return rtrn_value;
}
/*
 *  result += (-1)^(1+j) * matrix[0][j] * minor_j
 *  minor - determinant matrix without 0 row and j column
 */
double rec_determinant(matrix_t *A) {
  double result = 0;
  if (A->rows == 1) {
    result = A->matrix[0][0];
  } else if (A->rows == 2) {
    result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[1][0] * A->matrix[0][1];
  } else if (A->rows > 2) {
    int degree = 1;  //  (-1)^(1+j)
    for (int j = 0; j < A->rows; j++) {
      matrix_t lower_A;
      create_matrix(A->rows - 1, A->columns - 1, &lower_A);
      mtrx_minor(*A, &lower_A, 0, j);
      result += degree * A->matrix[0][j] * rec_determinant(&lower_A);
      degree = -degree;
      remove_matrix(&lower_A);
    }
  }
  return result;
}

int determinant(matrix_t *A, double *result) {
  mreturn_t rtrn_value = OK;
  if (!result || !mtrx_normal(A))
    rtrn_value = ERR_MATR;
  else if (A->columns != A->rows)
    rtrn_value = ERR_CALC;
  else
    *result = rec_determinant(A);
  return rtrn_value;
}

// minor and matrix of algebraic complements
int calc_complements(matrix_t *A, matrix_t *result) {
  mreturn_t rtrn_value = OK;
  if (!result || !mtrx_normal(A))
    rtrn_value = ERR_MATR;
  else if (A->columns != A->rows ||
           create_matrix(A->rows, A->columns, result))
    rtrn_value = ERR_CALC;
  else if (A->rows == 1)
    result->matrix[0][0] = 1;
  else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        matrix_t lower_A;
        create_matrix(A->rows - 1, A->columns - 1, &lower_A);
        mtrx_minor(*A, &lower_A, i, j);
        result->matrix[i][j] = rec_determinant(&lower_A) * powl(-1, i + j);
        remove_matrix(&lower_A);
      }
    }
  }
  return rtrn_value;
}

int inverse_matrix(matrix_t *A, matrix_t *result) {
  mreturn_t rtrn_value = OK;
  double det = 0;
  if (!result || !mtrx_normal(A))
    rtrn_value = ERR_MATR;
  else if (A->columns != A->rows || determinant(A, &det) || !det)
    rtrn_value = ERR_CALC;
  else {
    matrix_t tmp_result;
    rtrn_value = OK;
    calc_complements(A, result);
    transpose(result, &tmp_result);
    remove_matrix(result);
    mult_number(&tmp_result, 1 / det, result);
    remove_matrix(&tmp_result);
  }
  return rtrn_value;
}
