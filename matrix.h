#include <stdio.h>
#include <stdlib.h>

#define EPSILON 10e-9

void mem_err(void *ptr);
void value_err(void *ptr);

typedef struct {
	size_t rows, cols;
	double **data;
} Matrix;

typedef struct {
	size_t len;
	double *data;
} Vector;

void define_matrix(Matrix* matrix, double entries[], size_t len); // tested
void define_vector(Vector* vector, double entries[], size_t len);
void free_vector(Vector* vector);
void free_matrix(Matrix* matrix); // tested
void print_matrix(Matrix* matrix, int precision); // no need
void swap_rows(Matrix* matrix, int a, int b); // tested
void swap_cols(Matrix* matrix, int a, int b); // tested

Matrix* create_matrix(size_t rows, size_t cols); // tested
Matrix* null_matrix(size_t rows, size_t cols); // tested
Matrix* id_matrix(size_t n); // tested
Matrix* vector_to_matrix(Vector* vector); // not tested
Matrix* copy_matrix(Matrix* matrix); // tested
Matrix* symmetric_matrix(Matrix* matrix); // tested
Matrix* transpose_matrix(Matrix* matrix); // not tested
Matrix* inverse_matrix(Matrix* matrix); // not tested
Matrix* add_matrix(Matrix* a, Matrix* b); // tested
Matrix* sub_matrix(Matrix* a, Matrix* b); // tested
Matrix* mult_matrix(Matrix* a, Matrix* b); // tested - needs more though

Vector* create_vector(size_t len);
Vector* null_vector(size_t len);
Vector* matrix_to_vector(Matrix* matrix);
Vector* matrix_vector_mul(Matrix* matrix, Vector* vector);

int is_square_matrix(Matrix* matrix); // not tested
double determinant_matrix(Matrix* matrix); // not tested
int is_null_matrix(Matrix* matrix, double epsilon); // tested
int is_id_matrix(Matrix* matrix, double epsilon); // tested
int has_null_row(Matrix* matrix, double epsilon); // not tested
int has_null_col(Matrix* matrix, double epsilon); // not tested
int equal_matrix(Matrix* a, Matrix* b, double epsilon); // tested

double abs_d(double number); // partially tested - not much need though
