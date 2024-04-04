#include <stdio.h>
#include <stdlib.h>

#define EPSILON 10e-9

void mem_err(void *ptr);
void value_err(void *ptr);

typedef struct {
	int rows, cols;
	double **data;
} Matrix;

void define_matrix(Matrix* matrix, double entries[], int len); // tested
void free_matrix(Matrix* matrix); // tested
void print_matrix(Matrix* matrix, int precision); // no need
void swap_rows(Matrix* matrix, int a, int b); // tested
void swap_cols(Matrix* matrix, int a, int b); // tested

Matrix* create_matrix(int rows, int cols); // tested
Matrix* null_matrix(int rows, int cols); // tested
Matrix* id_matrix(int n); // tested
Matrix* vector_to_matrix(double vector[], int len); // not tested
Matrix* copy_matrix(Matrix* matrix); // tested
Matrix* symmetric_matrix(Matrix* matrix); // not tested
Matrix* transpose_matrix(Matrix* matrix); // not tested
Matrix* inverse_matrix(Matrix* matrix); // not tested
Matrix* add_matrix(Matrix* a, Matrix* b); // not tested
Matrix* sub_matrix(Matrix* a, Matrix* b); // not tested
Matrix* mult_matrix(Matrix* a, Matrix* b); // tested - needs more though

int is_square_matrix(Matrix* matrix); // not tested
int determinant_matrix(Matrix* matrix); // not tested
int is_null_matrix(Matrix* matrix, double epsilon); // tested
int is_id_matrix(Matrix* matrix, double epsilon); // tested
int has_null_row(Matrix* matrix, double epsilon); // not tested
int has_null_col(Matrix* matrix, double epsilon); // not tested
int equal_matrix(Matrix* a, Matrix* b, double epsilon); // tested

double abs_d(double number); // partially tested - not much need though
double* matrix_to_vector(Matrix* matrix); // not tested
double* matrix_vector_mul(Matrix* matrix, double vector[], int len); // not tested
