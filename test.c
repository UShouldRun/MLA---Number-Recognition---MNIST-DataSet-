// This test file isn't complete. Some tests show some bugs, but I still didn't check them to see if those were in the tests or in the matrix.c file functions.
// Even though I still have work to do on these unit tests, most of the tests give positive feedback.

#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EPSILON_TEST 10e-9
#define ROWS rand()%10 + 1
#define COLS rand()%10 + 1

int test_definition(Matrix* matrix, double entries[], size_t len);
int test_swap(Matrix* matrix, Matrix* swapped, int a, int b, int swap);
int test_transpose(Matrix* matrix, Matrix* transpose);
double time_elapsed(clock_t start, clock_t end);

void test_create_matrix();
void test_null_matrix();
void test_define_matrix();
void test_copy_matrix();
void test_symmetric_matrix();
void test_transpose_matrix();
void test_inverse_matrix();
void test_add_matrix();
void test_sub_matrix();
void test_mult_matrix();
void test_is_square_matrix();
void test_determinant_matrix();
void test_is_null_matrix();
void test_is_id_matrix();
void test_has_null_row();
void test_has_null_col();
void test_equal_matrix();
void test_create_vector();
void test_null_vector();
void test_define_vector();
void test_vector_to_matrix();
void test_matrix_to_vector();
void test_matrix_vector_mul();

int main() {
    srand(time(NULL));

    test_create_matrix();
    test_define_matrix();
    test_null_matrix();
    test_copy_matrix();
    test_symmetric_matrix();
    test_transpose_matrix();
    test_inverse_matrix();
    test_add_matrix();
    test_sub_matrix();
    test_mult_matrix();
    test_is_square_matrix();
    test_determinant_matrix();
    test_is_null_matrix();
    test_is_id_matrix();
    test_has_null_row();
    test_has_null_col();
    test_equal_matrix();
    test_define_vector();
    test_vector_to_matrix();
    test_matrix_vector_mul();
    test_is_square_matrix();

    return 0;
}


int test_definition(Matrix* matrix, double entries[], size_t len) {
    if (matrix->rows * matrix->cols != len) {
        value_err(NULL); // Check if the length matches
        return 0;
    }
    
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            if (entries[i * matrix->cols + j] != matrix->data[i][j]) {
                return 0; // Entry doesn't match
            }
        }
    }
    
    return 1; // All entries match
}

int test_swap(Matrix* matrix, Matrix* swapped, int a, int b, int swap) {
	if (!swap) {
		for (int i = 0; i < matrix->rows; i++)
		if (matrix->data[i][a] != swapped->data[i][b] || matrix->data[i][b] != swapped->data[i][a])
			return 0;
		return 1;
	}
	for (int j = 0; j < matrix->cols; j++)
		if (matrix->data[a][j] != swapped->data[b][j] || matrix->data[b][j] != swapped->data[a][j])
			return 0;
	return 1;
} 

int test_transpose(Matrix* matrix, Matrix* transpose) {
	for (int i = 0; i < matrix->rows; i++)
		for (int j = 0; j < matrix->cols; j++)
			if (matrix->data[i][j] - transpose->data[j][i]) return 0;
	return 1;
}

// Helper function to measure execution time
double time_elapsed(clock_t start, clock_t end) {
    return ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0; // Convert to milliseconds
}

void print_test_result(const char* test_name, int condition, double elapsed_time) {
    if (condition)
        printf("\033[0;32m"); // Set color to green for pass
    else
        printf("\033[0;31m"); // Set color to red for fail

    printf("Test %s: %s", test_name, condition ? "\u2713" : "\u2718");

    if (elapsed_time >= 0)
        printf(" - Execution time: %.5f ms", elapsed_time);

    printf("\033[0m\n"); // Reset color
}

void test_create_matrix() {
    printf("Running test_create_matrix...\n");
    clock_t start = clock();
    Matrix* matrix = create_matrix(ROWS, COLS);
    clock_t end = clock();
    int condition = matrix != NULL;
    free_matrix(matrix);
    print_test_result("create_matrix", condition, time_elapsed(start, end));
}

void test_null_matrix() {
    printf("Running test_null_matrix...\n");
    clock_t start = clock();
    Matrix* matrix = null_matrix(ROWS, COLS);
    clock_t end = clock();
    int condition = is_null_matrix(matrix, EPSILON_TEST);
    print_test_result("null_matrix", condition, time_elapsed(start, end));
}

void test_define_matrix() {
    printf("Running test_define_matrix...\n");
    clock_t start, end;
    int condition;
    const size_t rows =  ROWS;
    const size_t cols = COLS;

    double entries[rows * cols];
    for (int i = 0; i < rows * cols; ++i) entries[i] = (double)(rand() % 100);

    Matrix* matrix = create_matrix(rows, cols);
    start = clock();
    define_matrix(matrix, entries, rows * cols);
    end = clock();
    printf("Defined succesfully... Checking with the array...\n");

    condition = test_definition(matrix, entries, rows * cols);
    print_test_result("define_matrix", condition, time_elapsed(start, end));
    free_matrix(matrix);
}

void test_copy_matrix() {
    printf("Running test_copy_matrix...\n");
    size_t rows = ROWS;
    size_t cols = COLS;
    clock_t start, end;

    double entries[rows * cols];
    for (int i = 0; i < rows * cols; ++i) entries[i] = (double)(rand() % 100);

    Matrix* matrix = create_matrix(rows, cols);
    define_matrix(matrix, entries, rows * cols);

    start = clock();
    Matrix* copied_matrix = copy_matrix(matrix);
    end = clock();

    int are_equal = equal_matrix(matrix, copied_matrix, EPSILON);
    print_test_result("copy_matrix", are_equal, time_elapsed(start, end));
    free_matrix(matrix);
    free_matrix(copied_matrix);
}

void test_symmetric_matrix() {
    printf("Running test_symmetric_matrix...\n");
    size_t rows = ROWS;
    size_t cols = COLS;
    clock_t start, end;

    double entries[rows * cols];
    for (int i = 0; i < rows * cols; ++i) entries[i] = (double)(rand() % 100);

    Matrix* matrix = create_matrix(rows, cols);
    define_matrix(matrix, entries, rows * cols);

    start = clock();
    Matrix* symmetric = symmetric_matrix(matrix);
    end = clock();

    int is_not_null = symmetric != NULL;
    print_test_result("symmetric_matrix", is_not_null, time_elapsed(start, end)); // Print test result

    free_matrix(matrix);
    free_matrix(symmetric);
}

void test_transpose_matrix() {
    printf("Running test_transpose_matrix...\n");
    size_t rows = ROWS;
    size_t cols = COLS;
    clock_t start, end;

    double entries[rows * cols];
    for (int i = 0; i < rows * cols; ++i) entries[i] = (double)(rand() % 100);

    Matrix* matrix = create_matrix(rows, cols);
    define_matrix(matrix, entries, rows * cols);

    start = clock();
    Matrix* transposed = transpose_matrix(matrix);
    end = clock();

    int is_transposed = transposed->rows == cols && transposed->cols == rows;
    print_test_result("transpose_matrix", is_transposed, time_elapsed(start, end)); // Print test result

    free_matrix(matrix);
    free_matrix(transposed);
}

void test_inverse_matrix() {
    printf("Running test_inverse_matrix...\n");
    size_t size = ROWS;
    clock_t start, end;
    
    Matrix* matrix = create_matrix(size, size);
    double entries[size * size];
    for (int i = 0; i < size * size; ++i) entries[i] = (double)(rand() % 100);
    define_matrix(matrix, entries, size * size);

    start = clock();
    Matrix* inverse = inverse_matrix(matrix);
    end = clock();

    int is_not_null = inverse != NULL;
    print_test_result("inverse_matrix", is_not_null, time_elapsed(start, end));

    free_matrix(matrix);
    free_matrix(inverse);
}

void test_add_matrix() {
    printf("Running test_add_matrix...\n");
    size_t rows = ROWS;
    size_t cols = COLS;
    clock_t start, end;

    double entries1[rows * cols];
    double entries2[rows * cols];
    for (int i = 0; i < rows * cols; ++i) {
        entries1[i] = (double)(rand() % 100);
        entries2[i] = (double)(rand() % 100);
    }

    Matrix* matrix1 = create_matrix(rows, cols);
    Matrix* matrix2 = create_matrix(rows, cols);
    define_matrix(matrix1, entries1, rows * cols);
    define_matrix(matrix2, entries2, rows * cols);

    start = clock();
    Matrix* sum = add_matrix(matrix1, matrix2);
    end = clock();

    int is_not_null = sum != NULL;
    print_test_result("add_matrix", is_not_null, time_elapsed(start, end)); // Print test result

    free_matrix(matrix1);
    free_matrix(matrix2);
    free_matrix(sum);
}

void test_sub_matrix() {
    printf("Running test_sub_matrix...\n");
    size_t rows = ROWS;
    size_t cols = COLS; 
    clock_t start, end;

    double entries1[rows * cols];
    double entries2[rows * cols];
    for (int i = 0; i < rows * cols; ++i) {
        entries1[i] = (double)(rand() % 100);
        entries2[i] = (double)(rand() % 100);
    }

    Matrix* matrix1 = create_matrix(rows, cols);
    Matrix* matrix2 = create_matrix(rows, cols);
    define_matrix(matrix1, entries1, rows * cols);
    define_matrix(matrix2, entries2, rows * cols);

    start = clock();
    Matrix* difference = sub_matrix(matrix1, matrix2);
    end = clock();

    int is_not_null = difference != NULL;
    print_test_result("sub_matrix", is_not_null, time_elapsed(start, end)); // Print test result

    free_matrix(matrix1);
    free_matrix(matrix2);
    free_matrix(difference);
}

void test_mult_matrix() {
    printf("Running test_mult_matrix...\n");
    size_t rows1 = ROWS; 
    size_t cols1 = COLS;
    size_t cols2 = COLS + 1;
    clock_t start, end;

    double entries1[rows1 * cols1];
    double entries2[cols1 * cols2];
    for (int i = 0; i < rows1 * cols1; ++i) entries1[i] = (double)(rand() % 100);
    for (int i = 0; i < cols1 * cols2; ++i) entries2[i] = (double)(rand() % 100);

    Matrix* matrix1 = create_matrix(rows1, cols1);
    Matrix* matrix2 = create_matrix(cols1, cols2);
    define_matrix(matrix1, entries1, rows1 * cols1);
    define_matrix(matrix2, entries2, cols1 * cols2);

    start = clock();
    Matrix* product = mult_matrix(matrix1, matrix2);
    end = clock();

    int is_not_null = product != NULL;
    print_test_result("mult_matrix", is_not_null, time_elapsed(start, end)); // Print test result

    free_matrix(matrix1);
    free_matrix(matrix2);
    free_matrix(product);
}

void test_is_square_matrix() {
    printf("Running test_square_matrix...\n");
    size_t rows = ROWS;
    size_t cols = rows;
    clock_t start, end;

    Matrix* matrix = create_matrix(rows, cols);
    Matrix* matrix1 = create_matrix(rows, cols - 1);

    start = clock();
    int is_square1 = is_square_matrix(matrix);
    int is_square2 = is_square_matrix(matrix1);
    end = clock();

    print_test_result("is_square_matrix", is_square1*!is_square2, time_elapsed(start, end));

    free_matrix(matrix);
}

void test_determinant_matrix() {
    printf("Running test_determinant_matrix...\n");
    size_t size = ROWS;
    clock_t start, end;

    Matrix* matrix = create_matrix(size, size);
    double entries[size * size];
    for (int i = 0; i < size * size; ++i) entries[i] = (double)(rand() % 100);
    define_matrix(matrix, entries, size * size);

    start = clock();
    int determinant = determinant_matrix(matrix);
    end = clock();

    int is_calculated = determinant != 0;
    print_test_result("determinant_matrix", is_calculated, time_elapsed(start, end));

    free_matrix(matrix);
}

void test_is_null_matrix() {
    printf("Running test_is_null_matrix...\n");
    size_t rows = ROWS;
    size_t cols = COLS;
    clock_t start, end;

    Matrix* matrix = create_matrix(rows, cols);
    Matrix* matrix1 = create_matrix(rows, cols);
    double entries[rows * cols], entries1[rows * cols];
    for (int i = 0; i < rows * cols; ++i)  {
        entries[i] = 0;
	entries1[i] = rand()%10 + 1; 
    }
    define_matrix(matrix, entries, rows * cols);
    define_matrix(matrix1, entries1, rows * cols);

    start = clock();
    int is_null = is_null_matrix(matrix, EPSILON);
    end = clock();
    int is_null1 = is_null_matrix(matrix1, EPSILON);

    print_test_result("is_null_matrix", is_null*!is_null1, time_elapsed(start, end));

    free_matrix(matrix);
    free_matrix(matrix1);
}

void test_is_id_matrix() {
    printf("Running test_is_id_matrix...\n");
    size_t size = ROWS;
    clock_t start, end;

    Matrix* matrix = id_matrix(size);

    start = clock();
    int is_identity = is_id_matrix(matrix, EPSILON);
    end = clock();

    print_test_result("is_id_matrix", is_identity, time_elapsed(start, end));

    free_matrix(matrix);
}

void test_has_null_row() {
    printf("Running test_null_null_row...\n");
    size_t rows = ROWS;
    size_t cols = COLS;
    clock_t start, end;

    Matrix* matrix = create_matrix(rows, cols);
    int rand_row = rand()%rows;
    double entries[rows * cols];
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
	    if (i == rand_row) entries[i*cols + j] = 0;
	    else entries[i*cols + j] = (double)(rand() % 100);
	}
    define_matrix(matrix, entries, rows * cols);

    start = clock();
    int has_null = has_null_row(matrix, EPSILON);
    end = clock();

    print_test_result("has_null_row", has_null, time_elapsed(start, end));

    free_matrix(matrix);
}

void test_has_null_col() {
    printf("Running test_has_null_col...\n");
    size_t rows = ROWS;
    size_t cols = COLS;
    clock_t start, end;

    Matrix* matrix = create_matrix(rows, cols);
    int rand_col = rand()%cols;
    double entries[rows * cols];
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
	    if (j == rand_col) entries[i*cols + j] = 0;
	    else entries[i*cols + j] = (double)(rand() % 100);
	}
    define_matrix(matrix, entries, rows * cols);

    start = clock();
    int has_null = has_null_col(matrix, EPSILON);
    end = clock();

    print_test_result("has_null_col", has_null, time_elapsed(start, end));

    free_matrix(matrix);
}

void test_equal_matrix() {
    printf("Running test_equal_matrix...\n");
    size_t rows = ROWS;
    size_t cols = COLS;
    clock_t start, end;

    Matrix* matrix1 = create_matrix(rows, cols);
    Matrix* matrix2 = create_matrix(rows, cols);
    double entries1[rows * cols];
    double entries2[rows * cols];
    for (int i = 0; i < rows * cols; ++i) {
        entries1[i] = (double)(rand() % 100);
        entries2[i] = entries1[i] + 1;
    }
    define_matrix(matrix1, entries1, rows * cols);
    define_matrix(matrix2, entries2, rows * cols);

    start = clock();
    int are_equal1 = equal_matrix(matrix1, matrix2, EPSILON);
    int are_equal2 = equal_matrix(matrix1, matrix1, EPSILON);
    end = clock();

    print_test_result("equal_matrix", are_equal2 * !are_equal1, time_elapsed(start, end));

    free_matrix(matrix1);
    free_matrix(matrix2);
}

void test_define_vector() {
    printf("Running test_define_vector...\n");
    size_t len = ROWS;
    clock_t start, end;
    double entries[len];
    for (int i = 0; i < len; i++) entries[i] = rand() % 100;

    Vector* vector = create_vector(len);
    start = clock();
    define_vector(vector, entries, len);
    end = clock();

    int is_defined = 1;
    for (int i = 0; i < len; i++) {
        if (vector->data[i] != entries[i]) {
            is_defined = 0;
            break;
        }
    }

    print_test_result("define_vector", is_defined, time_elapsed(start, end));

    free_vector(vector);
}

void test_vector_to_matrix() {
    printf("Running test_vector_to_matrix...\n");
    size_t len = ROWS;
    clock_t start, end;
    double entries[len];
    for (int i = 0; i < len; i++) entries[i] = rand() % 100;

    Vector* vector = create_vector(len);
    define_vector(vector, entries, len);

    start = clock();
    Matrix* matrix = vector_to_matrix(vector);
    end = clock();

    int is_correct = matrix->rows == len && matrix->cols == 1;
    for (int i = 0; i < len; i++) {
        if (matrix->data[i][0] != entries[i]) {
            is_correct = 0;
            break;
        }
    }

    print_test_result("vector_to_matrix", is_correct, time_elapsed(start, end));

    free_vector(vector);
    free_matrix(matrix);
}

void test_matrix_to_vector() {
    printf("Running test_matrix_to_vector...\n");
    clock_t start, end;
    size_t n = ROWS, m = 1;
    double entries[n * m];
    for (int i = 0; i < n; i++) entries[i] = rand() % 100;

    char *test = "matrix_to_vector";
    Matrix *matrix = create_matrix(n, m);
    define_matrix(matrix, entries, n * m);

    start = clock();
    Vector *vector = matrix_to_vector(matrix);
    end = clock();

    printf("Resulting vector:\n");
    for (int i = 0; i < vector->len; i++) printf("%.2f ", vector->data[i]);
    printf("\n");

    print_test_result(test, vector != NULL, time_elapsed(start, end));

    free_matrix(matrix);
    free_vector(vector);
}

void test_matrix_vector_mul() {
    printf("Running test_matrix_vector_mul...\n");
    size_t rows = ROWS;
    size_t cols = COLS;
    size_t len = cols;
    clock_t start, end;

    Matrix* matrix = create_matrix(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix->data[i][j] = rand() % 100;
        }
    }

    double entries[len];
    for (int i = 0; i < len; i++) entries[i] = rand() % 100;
    Vector* vector = create_vector(len);
    define_vector(vector, entries, len);

    start = clock();
    Vector* result = matrix_vector_mul(matrix, vector);
    end = clock();

    int is_correct = 1;
    for (int i = 0; i < rows; i++) {
        double sum = 0;
        for (int j = 0; j < cols; j++) {
            sum += matrix->data[i][j] * entries[j];
        }
        if (result->data[i] != sum) {
            is_correct = 0;
            break;
        }
    }

    print_test_result("matrix_vector_mul", is_correct, time_elapsed(start,end));

    free_matrix(matrix);
    free_vector(vector);
    free_vector(result);
}
