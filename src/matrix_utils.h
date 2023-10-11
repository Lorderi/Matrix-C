#if !defined(MATRIX_LONG_H)
#define MATRIX_LONG_H

int over_precision(double A);
int mtrx_normal(matrix_t *A);
int mtrx_equal_borders(matrix_t *A, matrix_t *B);
double addition(double A, double B);
double subtraction(double A, double B);
void mtrx_minor(matrix_t A, matrix_t *result, int row, int column);

#endif  //  MATRIX_LONG_H
