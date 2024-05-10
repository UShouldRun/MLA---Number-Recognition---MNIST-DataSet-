#ifndef NLA_H
#define NLA_H

#include <stdio.h>
#include <stdlib.h>

#define EPSILON 10e-9

void mem_err(void *ptr, int line);
void value_err(void *ptr);

typedef struct {
	size_t rows, cols;
	double **data;
} Matrix;

typedef struct {
	size_t len;
	double *data;
} Vector;

void define_matrix(Matrix* matrix, double entries[], size_t len);
void define_vector(Vector* vector, double entries[], size_t len);
void free_vector(Vector* vector);
void free_matrix(Matrix* matrix);
void print_matrix(Matrix* matrix, int precision);
void swap_rows(Matrix* matrix, int a, int b);
void swap_cols(Matrix* matrix, int a, int b);

Matrix* create_matrix(size_t rows, size_t cols);
Matrix* null_matrix(size_t rows, size_t cols);
Matrix* id_matrix(size_t n);
Matrix* vector_to_matrix(Vector* vector);
Matrix* copy_matrix(Matrix* matrix);
Matrix* symmetric_matrix(Matrix* matrix);
Matrix* transpose_matrix(Matrix* matrix);
Matrix* inverse_matrix(Matrix* matrix);
Matrix* add_matrix(Matrix* a, Matrix* b);
Matrix* sub_matrix(Matrix* a, Matrix* b);
Matrix* mult_matrix(Matrix* a, Matrix* b);

Vector* create_vector(size_t len);
Vector* null_vector(size_t len);
Vector* copy_vector(Vector* vector);
Vector* matrix_to_vector(Matrix* matrix);
Vector* matrix_vector_mul(Matrix* matrix, Vector* vector);
Vector* cross_product_3d(Vector* v, Vector* u);

int is_square_matrix(Matrix* matrix);
int is_null_matrix(Matrix* matrix, double epsilon);
int is_id_matrix(Matrix* matrix, double epsilon);
int has_null_row(Matrix* matrix, double epsilon);
int has_null_col(Matrix* matrix, double epsilon);
int equal_matrix(Matrix* a, Matrix* b, double epsilon);

long factorial(long n);

double pow_int(double x, long n);
double exp(double x);
double sigmoid(double x);
double abs_d(double number);
double determinant_matrix(Matrix* matrix);
double dot_product_vector(Vector* v, Vector* u);
double norm_vector(Vector* vector);

#endif
