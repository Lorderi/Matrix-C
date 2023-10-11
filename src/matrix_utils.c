#include "matrix.h"

int over_precision(double A) { return fabsl(A) >= EPS; }

int mtrx_normal(matrix_t *A) {
  return A && A->matrix && A->rows > 0 && A->columns > 0;
}

int mtrx_equal_borders(matrix_t *A, matrix_t *B) {
  return A && B && (A->rows == B->rows) && (A->columns == B->columns);
}

double addition(double A, double B) { return A + B; }

double subtraction(double A, double B) { return A - B; }

// do matrix without row and column
void mtrx_minor(matrix_t A, matrix_t *result, int row, int column) {
  for (int i = 0, di = 0; i < A.rows - 1; i++) {
    if (i == row) di = 1;
    for (int j = 0, dj = 0; j < A.rows - 1; j++) {
      if (j == column) dj = 1;
      result->matrix[i][j] = A.matrix[i + di][j + dj];
    }
  }
}
