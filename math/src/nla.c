// More functions and changes will be made in the future

#include "../include/nla.h"

void mem_err(void *ptr, int line) {
     if (ptr == NULL) {
          fprintf(stderr, "Runtime error : math/src/nla.c : %d : memory allocation failed\n", line);
          exit(ENOMEM);
     }
}

void value_err(void *ptr, int line) {
     if (ptr == NULL) {
          fprintf(stderr, "Runtime error : math/src/nla.c : %d : value error\n", line);
          exit(EINVAL);
     }
}

void define_matrix(Matrix* matrix, double entries[], size_t len) {
     if (matrix == NULL) return;
     if (len != matrix->rows * matrix->cols) value_err(NULL, 21);
     for (int i = 0; i < matrix->rows; i++)
          for (int j = 0; j < matrix->cols; j++)
               matrix->data[i][j] = entries[i*matrix->cols + j];
}

void define_vector(Vector* vector, double entries[], size_t len) {
     if (vector == NULL) return;
     if (vector->len != len) value_err(NULL, 29);
     for (int i = 0; i < vector->len; i++) vector->data[i] = entries[i];
}

void free_matrix(Matrix* matrix) {
     if (matrix == NULL) return;
     for (int i = 0; i < matrix->rows; i++) free(matrix->data[i]);
     free(matrix->data);
     free(matrix);
}

void free_vector(Vector* vector) {
     if (vector == NULL) return;
     free(vector->data);
     free(vector);
}

void print_matrix(Matrix* matrix, int precision) {
     if (matrix == NULL) return;
     for (int i = 0; i < matrix->rows; i++) {
          printf("( ");
          for (int j = 0; j < matrix->cols; j++) {
               printf("%.*lf ", precision, matrix->data[i][j]);
          }
          printf(")\n");
     }
}

void swap_rows(Matrix* matrix, int a, int b) {
     if (matrix == NULL) return;
     if (a < 0 || b < 0 || a >= matrix->rows || b >= matrix->rows) value_err(NULL, 59);
     for (int j = 0; j < matrix->cols; j++) {
          double temp = matrix->data[a][j];
          matrix->data[a][j] = matrix->data[b][j];
          matrix->data[b][j] = temp;
     }
}
void swap_cols(Matrix* matrix, int a, int b) {
     if (matrix == NULL) return;
     if (a < 0 || b < 0 || a >= matrix->cols || b >= matrix->cols) value_err(NULL, 68);
     for (int i = 0; i < matrix->rows; i++) { 
          double temp = matrix->data[i][a];
          matrix->data[i][a] = matrix->data[i][b];
          matrix->data[i][b] = temp;
     } 
}

Matrix* create_matrix(size_t rows, size_t cols) { 
     if (rows == 0 || cols == 0) value_err(NULL, 77);
     Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
     if (matrix == NULL) mem_err(NULL, 79);

     matrix->rows = rows;
     matrix->cols = cols;

     matrix->data = (double**)malloc(rows * sizeof(double*));
     if (matrix->data == NULL) mem_err(NULL, 85);

     for (int i = 0; i < rows; i++) {
          matrix->data[i] = (double*)malloc(cols * sizeof(double));
          if (matrix->data[i] == NULL) mem_err(NULL, 89);
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
     if (vector == NULL) return NULL;
     Matrix* vector_mat = null_matrix(vector->len,1);
     define_matrix(vector_mat, vector->data, vector->len);
     return vector_mat;
}

Matrix* copy_matrix(Matrix* matrix) {
     if (matrix == NULL) return NULL;
     Matrix* copy = create_matrix(matrix->rows, matrix->cols);
     for (int i = 0; i < matrix->rows; i++)
          for (int j = 0; j < matrix->cols; j++)
               copy->data[i][j] = matrix->data[i][j];
     return copy;
}

Matrix* symmetric_matrix(Matrix* matrix) {
     if (matrix == NULL) return NULL;
     Matrix* symetric = create_matrix(matrix->rows, matrix->cols);
     for (int i = 0; i < symetric->rows; i++)
          for (int j = 0; j < symetric->cols; j++)
               symetric->data[i][j] = -matrix->data[i][j];
     return symetric;
}

Matrix* transpose_matrix(Matrix* matrix) {
     if (matrix == NULL) return NULL;
     Matrix* transpose = create_matrix(matrix->cols, matrix->rows);
     for (int i = 0; i < matrix->rows; i++)
          for (int j = 0; j < matrix->cols; j++)
               transpose->data[j][i] = matrix->data[i][j];
     return transpose;
}

Matrix* inverse_matrix(Matrix* matrix) {
     if (matrix == NULL) return NULL;
     if (!is_square_matrix(matrix)) return NULL;
     if (!determinant_matrix(matrix)) return NULL;
     if (is_id_matrix(matrix, EPSILON)) return id_matrix(matrix->rows);
     if (matrix->rows == 1) {
          Matrix* inverse = null_matrix(matrix->rows, matrix->cols);
          inverse->data[0][0] = 1/matrix->data[0][0];
          return inverse;
     }

     Matrix* copy = copy_matrix(matrix);
     Matrix* id = id_matrix(matrix->rows);

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

          if (a_jj != 1)
                for (int k = 0; k < copy->cols; k++) {
                    copy->data[j][k] /= a_jj;
                    id->data[j][k] /= a_jj;
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
     if (a == NULL || b == NULL) return NULL;
     if (!(a->rows == b->rows && a->cols == b->cols)) return NULL;
     Matrix* c = create_matrix(a->rows,a->cols);
     for (int i = 0; i < c->rows; i++)
          for (int j = 0; j < c->cols; j++)
               c->data[i][j] = a->data[i][j] + b->data[i][j];
     return c;
}

Matrix* sub_matrix(Matrix* a, Matrix* b) {
    if (a == NULL || b == NULL) return NULL;
    return add_matrix(a, symmetric_matrix(b));
}

Matrix* mult_matrix(Matrix* a, Matrix* b) {
     if (a == NULL || b == NULL) return NULL;
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
     if (!len) return NULL;
     Vector* vector = (Vector*)malloc(sizeof(Vector));
     if (vector == NULL) mem_err(NULL, 226);

     vector->len = len;
     vector->data = (double*)malloc(vector->len * sizeof(double));
     if (vector->data == NULL) mem_err(NULL, 230);

     return vector;
}

Vector* null_vector(size_t len) {
     Vector* vector = create_vector(len);
     for (int i = 0; i < vector->len; i++) vector->data[i] = 0;
     return vector;
}

Vector* copy_vector(Vector* vector) {
     if (vector == NULL) return NULL;
     Vector* copy = create_vector(vector->len);
     for (int i = 0; i < vector->len; i++) copy->data[i] = vector->data[i];
     return copy;
}

Vector* matrix_to_vector(Matrix* matrix) {
     if (matrix == NULL) return NULL;
     if (matrix->cols != 1) value_err(NULL, 250);
     Vector* vector = null_vector(matrix->rows);
     for (int i = 0; i < matrix->rows; i++) vector->data[i] = matrix->data[i][0];
     return vector;
}

Vector* matrix_vector_mul(Matrix* matrix, Vector* vector) {
     if (matrix == NULL || vector == NULL) return NULL;
     if (matrix->cols != vector->len) value_err(NULL, 258);
     Matrix* vec_mat = vector_to_matrix(vector);
     Matrix* result = mult_matrix(matrix, vec_mat);
     Vector* result_vector = matrix_to_vector(result);
     free_matrix(vec_mat);
     free_matrix(result);
     return result_vector;
}

Vector* cross_product_3d(Vector* v, Vector* u) {
    if (v == NULL || u == NULL) return NULL;
    if (v->len != 3 || u->len != 3) value_err(NULL, 269);
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
        w->data[j] = (j % 2 ? -1 : 1) * determinant_matrix(sub_matrix);
        free_matrix(sub_matrix);
    }

    return w;
}

int is_square_matrix(Matrix* matrix) {
    if (matrix == NULL) value_err(NULL, 289);
    return matrix->rows == matrix->cols;
}

int is_null_matrix(Matrix* matrix, double epsilon) {
     if (matrix == NULL) value_err(NULL, 323);
     Matrix* null_mat = null_matrix(matrix->rows,matrix->cols);
     int test = equal_matrix(matrix, null_mat, epsilon);
     free(null_mat);
     return test;
}

int is_id_matrix(Matrix* matrix, double epsilon) { 
     if (matrix == NULL) value_err(NULL, 331);
     if (matrix->rows != matrix->cols) return 0;
     Matrix* id = id_matrix(matrix->rows);
     int test = equal_matrix(matrix, id, epsilon);
     free(id);
     return test;
}

int has_null_row(Matrix* matrix, double epsilon) {
     if (matrix == NULL) value_err(NULL, 340);
     for (int i = 0; i < matrix->rows; i++) {
          unsigned count = 0;
          for (int j = 0; j < matrix->cols; j++)
               if (abs_d(matrix->data[i][j]) < epsilon) count++;
          if (count == matrix->cols) return 1;
     }
     return 0;
}

int has_null_col(Matrix* matrix, double epsilon) {
     if (matrix == NULL) value_err(NULL, 351);
     for (int j = 0; j < matrix->cols; j++) {
          unsigned count = 0;
          for (int i = 0; i < matrix->rows; i++)
               if (abs_d(matrix->data[i][j]) < epsilon) count++;
          if (count == matrix->rows) return 1;
     }
     return 0;
}

int equal_matrix(Matrix* a, Matrix* b, double epsilon) {
     if (a == NULL || b == NULL) value_err(NULL, 362);
     if (a->rows != b->rows || a->cols != b->cols) return 0;
     for (int i = 0; i < a->rows; i++)
          for (int j = 0; j < a->cols; j++)
               if (abs_d(a->data[i][j] - b->data[i][j]) > epsilon) return 0;
     return 1;
}

double determinant_matrix(Matrix* matrix) {
    if (matrix == NULL) value_err(NULL, 294);
    if (!is_square_matrix(matrix)) value_err(NULL, 295);
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

double dot_product_vector(Vector* v, Vector* u) {
    if (v == NULL || u == NULL) value_err(NULL, 412);
    if (v->len != u->len) value_err(NULL, 413);
    double dot_product = 0;
    for (int i = 0; i < v->len; i++) dot_product += v->data[i] * u->data[i];
    return dot_product;
}

double norm_vector(Vector* vector) {
    double sq_norm = 0;
    for (int i = 0; i < vector->len; i++) sq_norm += vector->data[i] * vector->data[i];
    return sqrt_d(sq_norm);
}
