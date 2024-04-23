// More functions and changes will be made in the future

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

void define_matrix(Matrix* matrix, double entries[], size_t len);
void define_vector(Vector* vector, double entries[], size_t len);
void free_matrix(Matrix* matrix);
void free_vector(Vector* vector);
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

void mem_err(void *ptr) {
     if (ptr == NULL) {
          printf("Memory allocation failed\n");
          exit(EXIT_FAILURE);
     }
}

void value_err(void *ptr) {
     if (ptr == NULL) {
          printf("Value error\n");
          exit(EXIT_FAILURE);
     }
}

void define_matrix(Matrix* matrix, double entries[], size_t len) {
     if (len != matrix->rows * matrix->cols) value_err(NULL);
     for (int i = 0; i < matrix->rows; i++)
          for (int j = 0; j < matrix->cols; j++)
               matrix->data[i][j] = entries[i*matrix->cols + j];
}

void define_vector(Vector* vector, double entries[], size_t len) {
     if (vector->len != len) value_err(NULL);
     for (int i = 0; i < vector->len; i++) vector->data[i] = entries[i];
}

void free_matrix(Matrix* matrix) {
     for (int i = 0; i < matrix->rows; i++) free(matrix->data[i]);
     free(matrix->data);
     free(matrix);
}

void free_vector(Vector* vector) {
     free(vector->data);
     free(vector);
}

void print_matrix(Matrix* matrix, int precision) {
     for (int i = 0; i < matrix->rows; i++) {
          printf("( ");
          for (int j = 0; j < matrix->cols; j++) {
               printf("%.*lf ", precision, matrix->data[i][j]);
          }
          printf(")\n");
     }
}

void swap_rows(Matrix* matrix, int a, int b) {
     if (a < 0 || b < 0 || a >= matrix->rows || b >= matrix->rows) value_err(NULL);
     for (int j = 0; j < matrix->cols; j++) {
          double temp = matrix->data[a][j];
          matrix->data[a][j] = matrix->data[b][j];
          matrix->data[b][j] = temp;
     }
}
void swap_cols(Matrix* matrix, int a, int b) {
     if (a < 0 || b < 0 || a >= matrix->cols || b >= matrix->cols) value_err(NULL);
     for (int i = 0; i < matrix->rows; i++) { 
          double temp = matrix->data[i][a];
          matrix->data[i][a] = matrix->data[i][b];
          matrix->data[i][b] = temp;
     } 
}

Matrix* create_matrix(size_t rows, size_t cols) { 
     Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
     mem_err(matrix);

     matrix->rows = rows;
     matrix->cols = cols;

     matrix->data = (double**)malloc(rows * sizeof(double*));
     mem_err(matrix->data);

     for (int i = 0; i < rows; i++) {
          matrix->data[i] = (double*)malloc(cols * sizeof(double));
          mem_err(matrix->data[i]);
     }

     return matrix;
}

Matrix* null_matrix(size_t rows, size_t cols) { 
     Matrix* matrix = create_matrix(rows, cols);
     double entries[rows*cols];
     for (int i = 0; i < rows*cols; i++) entries[i] = 0;
     define_matrix(matrix, entries, rows*cols);
     return matrix;
}

Matrix* id_matrix(size_t n) {
     Matrix* id = null_matrix(n,n);
     for (int i = 0; i < n; i++) id->data[i][i] = 1.;
     return id;
}

Matrix* vector_to_matrix(Vector* vector) {
     Matrix* vector_mat = null_matrix(vector->len,1);
     define_matrix(vector_mat, vector->data, vector->len);
     return vector_mat;
}

Matrix* copy_matrix(Matrix* matrix) {
     Matrix* copy = create_matrix(matrix->rows, matrix->cols);
     for (int i = 0; i < matrix->rows; i++)
          for (int j = 0; j < matrix->cols; j++)
               copy->data[i][j] = matrix->data[i][j];
     return copy;
}

Matrix* symmetric_matrix(Matrix* matrix) {
     Matrix* symetric = create_matrix(matrix->rows, matrix->cols);
     for (int i = 0; i < symetric->rows; i++)
          for (int j = 0; j < symetric->cols; j++)
               symetric->data[i][j] = -matrix->data[i][j];
     return symetric;
}

Matrix* transpose_matrix(Matrix* matrix) {
     Matrix* transpose = create_matrix(matrix->cols, matrix->rows);
     for (int i = 0; i < matrix->rows; i++)
          for (int j = 0; j < matrix->cols; j++)
               transpose->data[j][i] = matrix->data[i][j];
     return transpose;
}

Matrix* inverse_matrix(Matrix* matrix) {
     if (!is_square_matrix(matrix)) return NULL;
     if (!determinant_matrix(matrix)) return NULL;
     if (is_id_matrix(matrix, EPSILON)) return id_matrix(matrix->rows);
     if (matrix->rows == 1) {
          Matrix* inverse = null_matrix(matrix->rows, matrix->cols);
          inverse->data[0][0] = 1/matrix->data[0][0];
          return inverse;
     }

     Matrix* copy = copy_matrix(matrix);
     if (copy == NULL) return NULL;

     Matrix* id = id_matrix(matrix->rows);
     if (id == NULL) {
          free_matrix(copy);
          return NULL;
     }

     for (int j = 0; j < copy->cols && !is_id_matrix(copy, EPSILON); j++) {
          double a_jj = copy->data[j][j];

          if (!a_jj) {
               int i = j;
               while (a_jj == 0) {
                    i++;
                    if (i >= copy->rows) {
                        free_matrix(copy);
                        free_matrix(id);
                        return NULL;
                    }
                    a_jj = copy->data[i][j];
               }
               swap_rows(copy, i, j);
               swap_rows(id, i, j);
          }

          if (a_jj != 1) {
                for (int k = 0; k < copy->cols; k++) {
                    copy->data[j][k] /= a_jj;
                    id->data[j][k] /= a_jj;
               }
          }

          for (int i = 0; i < copy->rows; i++) {
               double a_ij = copy->data[i][j];
               for (int k = 0; k < copy->cols && i != j; k++) {
                    copy->data[i][k] -= copy->data[j][k] * a_ij;
                    id->data[i][k] -= id->data[j][k] * a_ij;
               }
          }
     }

     free_matrix(copy);
     return id;
}

Matrix* add_matrix(Matrix* a, Matrix* b) {
     if (!(a->rows == b->rows && a->cols == b->cols)) return NULL;
     Matrix* c = create_matrix(a->rows,a->cols);
     for (int i = 0; i < c->rows; i++)
          for (int j = 0; j < c->cols; j++)
               c->data[i][j] = a->data[i][j] + b->data[i][j];
     return c;
}

Matrix* sub_matrix(Matrix* a, Matrix* b) { return add_matrix(a, symmetric_matrix(b)); }

Matrix* mult_matrix(Matrix* a, Matrix* b) {
     if (a->cols - b->rows) return NULL;
     if (is_null_matrix(a, EPSILON) || is_null_matrix(b, EPSILON)) return null_matrix(a->rows,b->cols);
     if (is_id_matrix(a, EPSILON)) return copy_matrix(b);
     if (is_id_matrix(b, EPSILON)) return copy_matrix(a);
     Matrix* c = null_matrix(a->rows,b->cols);
     for (int i = 0; i < c->rows; i++)
          for (int j = 0; j < c->cols; j++)
               for (int k = 0; k < a->cols; k++)
                    c->data[i][j] += a->data[i][k] * b->data[k][j];
     return c;
}

Vector* create_vector(size_t len) {
     if (!len) value_err(NULL);
     Vector* vector = (Vector*)malloc(sizeof(Vector));
     mem_err(vector);

     vector->len = len;
     vector->data = (double*)malloc(vector->len * sizeof(double));
     mem_err(vector->data);

     return vector;
}

Vector* null_vector(size_t len) {
     Vector* vector = create_vector(len);
     for (int i = 0; i < vector->len; i++) vector->data[i] = 0;
     return vector;
}

Vector* copy_vector(Vector* vector) {
    Vector* copy = create_vector(vector->len);
    for (int i = 0; i < vector->len; i++) copy->data[i] = vector->data[i];
    return copy;
}

Vector* matrix_to_vector(Matrix* matrix) {
    if (matrix->cols != 1) value_err(NULL);
    Vector* vector = null_vector(matrix->rows);
    for (int i = 0; i < matrix->rows; i++) vector->data[i] = matrix->data[i][0];
    return vector;
}

Vector* matrix_vector_mul(Matrix* matrix, Vector* vector) {
    if (matrix->cols != vector->len) value_err(NULL);
    Matrix* vec_mat = vector_to_matrix(vector);
    Matrix* result = mult_matrix(matrix, vec_mat);
    Vector* result_vector = matrix_to_vector(result);
    free_matrix(vec_mat);
    free_matrix(result);
    return result_vector;
}

Vector* cross_product_3d(Vector* v, Vector* u) {
    assert(v->len != 3);
    Vector* w = create_vector(v->len);

    for (int j = 0; j < 3; j++) {
        Matrix* sub_matrix = create_matrix(2,2);
        for (int i = 1; i < 3; i++) {
            for (int k = 0; k < 3; k++)
                if (j != k) {
                    if (i == 2) sub_matrix->data[i][k - (k > j ? 1 : 0)] = v->data[k];
                    else sub_matrix->data[i][k - (k > j ? 1 : 0)] = u->data[k];
                }
        }
        w->data[i] = (j % 2 ? -1 : 1) * determinant_matrix(sub_matrix);
        free_matrix(sub_matrix);
    }

    return w;
}

int is_square_matrix(Matrix* matrix) { return matrix->rows == matrix->cols; }

double determinant_matrix(Matrix* matrix) {
    if (!is_square_matrix(matrix)) value_err(NULL);
    if (matrix->rows == 1) return matrix->data[0][0];
    if (has_null_row(matrix, EPSILON) || has_null_col(matrix, EPSILON)) return 0;
    if (is_id_matrix(matrix, EPSILON)) return 1;

    double apply_la_place(Matrix* mat) {
        if (mat->rows == 2) return mat->data[0][0] * mat->data[1][1] - mat->data[1][0] * mat->data[0][1];

        double det = 0;
        for (int j = 0; j < mat->cols; j++) {
            Matrix* sub_mat = null_matrix(mat->rows - 1, mat->cols - 1);
            for (int q = 0; q < mat->cols; q++)
                for (int p = 1; p < mat->rows; p++)
                    if (q != j) sub_mat->data[p - 1][q - (q > j ? 1 : 0)] = mat->data[p][q];
            int sign = j % 2 ? -1 : 1;
            det += sign * mat->data[0][j] * apply_la_place(sub_mat);
            free(sub_mat);
        }
        return det;
    }

    Matrix* determinant = copy_matrix(matrix);
    double det = apply_la_place(determinant);
    free_matrix(determinant);
    return det;
}

int is_null_matrix(Matrix* matrix, double epsilon) {
     Matrix* null_mat = null_matrix(matrix->rows,matrix->cols);
     int test = equal_matrix(matrix, null_mat, epsilon);
     free(null_mat);
     return test;
}

int is_id_matrix(Matrix* matrix, double epsilon) { 
     if (matrix->rows != matrix->cols) return 0;
     Matrix* id = id_matrix(matrix->rows);
     int test = equal_matrix(matrix, id, epsilon);
     free(id);
     return test;
}

int has_null_row(Matrix* matrix, double epsilon) {
     for (int i = 0; i < matrix->rows; i++) {
          unsigned count = 0;
          for (int j = 0; j < matrix->cols; j++)
               if (abs_d(matrix->data[i][j]) < epsilon) count++;
          if (count == matrix->cols) return 1;
     }
     return 0;
}

int has_null_col(Matrix* matrix, double epsilon) {
     for (int j = 0; j < matrix->cols; j++) {
          unsigned count = 0;
          for (int i = 0; i < matrix->rows; i++)
               if (abs_d(matrix->data[i][j]) < epsilon) count++;
          if (count == matrix->rows) return 1;
     }
     return 0;
}

int equal_matrix(Matrix* a, Matrix* b, double epsilon) {
     if (a->rows != b->rows || a->cols != b->cols) return 0;
     for (int i = 0; i < a->rows; i++)
          for (int j = 0; j < a->cols; j++)
               if (abs_d(a->data[i][j] - b->data[i][j]) > epsilon) return 0;
     return 1;
}

long factorial(long n) {
    long fact = 1;
    for (long i = 1; i < n; i++) fact *= i;
    return fact;
}

double pow_int(double x, long n) {
    if (x == 1) return 1;
    if (x == 0) return 0;
    if (x == -1) return n % 2 ? -1 : 1;
    double result = 1;
    double r = n < 0 ? 1/x : x;
    n = n < 0 ? -n : n;
    for (long i = 0; i < n; i++) result *= r;
    return 
}

double exp(double x) {
    long n = 0;
    double sum = 0, prev;
    do {
    prev = sum;
    sum += pow_int(x, n) / factorial(n);
    n++;
    } while(abs_d(sum - prev) > EPSILON)
    return sum;
}

double sigmoid(double x) { return 1/(1 + exp(-x)); }

double abs_d(double number) { return number < 0 ? -number : number; } 

double sqrt_d(double number) {
    double x0 = number/2;
    double newtons_method(double x_n, int iteration) {
        if (iteration == 50) return prev;
        return newtons_method(x_n/2 - x0/(2 * x_n), ++iteration);
    }
    return newtons_method(x0, 0);
}

double dot_product_vector(Vector* v, Vector* u) {
    assert(v->len != u->len);
    double dot_product = 0;
    for (int i = 0; i < v->len; i++) dot_product += v->data[i] * u->data[i];
    return dot_product;
}

double norm_vector(Vector* vector) {
    double sq_norm = 0;
    for (int i = 0; i < vector->len; i++) sq_norm += vector->data[i] * vector->data[i];
    return sqrt_d(sq_norm);
}
