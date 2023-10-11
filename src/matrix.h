#if !defined(MATRIX_H)
#define MATRIX_H

#include <malloc.h>
#include <math.h>
#include <string.h>

#define EPS 1e-7

typedef enum { FAILURE, SUCCESS } meq_t;
typedef enum { OK, ERR_MATR, ERR_CALC } mreturn_t;

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

#include "matrix_utils.h"

int create_matrix(int rows, int columns, matrix_t *result);
void remove_matrix(matrix_t *A);
int eq_matrix(matrix_t *A, matrix_t *B);
int sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int mult_number(matrix_t *A, double number, matrix_t *result);
int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int transpose(matrix_t *A, matrix_t *result);
int calc_complements(matrix_t *A, matrix_t *result);
int determinant(matrix_t *A, double *result);
int inverse_matrix(matrix_t *A, matrix_t *result);

#endif  //  MATRIX_H
