# Matrix library

Realize the main actions with matrices: create_matrix (creation), remove_matrix (cleaning and destruction), eq_matrix (comparison), sum_matrix (addition), sub_matrix (subtraction), mult_matrix (multiplication), mult_number (multiplication by a number), transpose (transposition), determinant (calculating the determinant), calc_complements (calculating the matrix of algebraic complements), inverse_matrix (finding the inverse matrix). 

- C11 standard using gcc compiler 
- Focus on the POSIX.1-2017 standard
- Google Style
- Static library (with matrix.h header file)
- Structured programming
- Full unit-test coverage of library functions (check.h)
- Makefile for library and test builds (with all, clean, test, matrix.a, gcov_report targets)
- The gcov_report target should generate a gcov report as an html page. For this purpose, unit tests must be run with gcov flags 
- Checked accuracy of the fractional part - 6 decimal places.

### Matrix structure in C:

```c
typedef struct matrix_struct {
    double** matrix;
    int rows;
    int columns;
} matrix_t;
```

## Operations on matrices

All operations (except matrix comparison) must return the resulting code:  
- 0 - OK
- 1 - Error, incorrect matrix   
- 2 - Calculation error (matrix sizes do not match; matrix for which calculations cannot be performed, etc.)

### Create matrix (create_matrix)

```c
int create_matrix(int rows, int columns, matrix_t *result);
```

### Clearing matrices (remove_matrix)

```c
void remove_matrix(matrix_t *A);
```

### Matrix comparison (eq_matrix)

```c
#define SUCCESS 1
#define FAILURE 0

int eq_matrix(matrix_t *A, matrix_t *B);
```

### Adding (sum_matrix) and subtracting matrices (sub_matrix)

```c
int sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
```

### Multiplication of a matrix by a number (mult_number). Multiplication of two matrices (mult_matrix)

```c
int mult_number(matrix_t *A, double number, matrix_t *result);
int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
```

### Transpose matrix (transpose)

```c
int transpose(matrix_t *A, matrix_t *result);
```

### Matrix minor and matrix of algebraic complements (calc_complements)

```c
int calc_complements(matrix_t *A, matrix_t *result);
```

### Matrix determinant (determinant)

```c
int determinant(matrix_t *A, double *result);
```

### Inverse matrix (inverse_matrix)

```c
int inverse_matrix(matrix_t *A, matrix_t *result);
```
