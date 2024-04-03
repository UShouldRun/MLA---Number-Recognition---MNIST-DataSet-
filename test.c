#include "matrix.h"
#include <time.h>

int test_definition(Matrix* matrix, double entries[], int len) {
	for (int i = 0; i < matrix->rows; i++)
		for (int j = 0; j < matrix->cols; j++)
			if (entries[i*matrix->rows + j] != matrix->data[i][j]) return 0;
	return 1;
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

void print_test(char* test, int condition, double delta_time) {
	if (condition) printf("\033[0;32m"); // Set color to green
	else printf("\033[0;31m"); // Set color to red

	printf("Test %s: %s", test, condition ? "\u2713" : "\u2718");
	if (delta_time == -1) { printf("\n"); return; }

	printf("\033[0;33m"); // Set color to yellow
	printf(" - CPU time: %.18lf\n", delta_time);	

	printf("\033[0m"); // Reset color
}

void test_matrix(double entries[], int len, int n, int m) {
	clock_t start, end;

	char* test1 = "create_matrix / null_matrix";
	start = clock();	
	Matrix* matrix = create_matrix(n,m);
	end = clock();
	print_test(test1, 1, (double) (end - start) / CLOCKS_PER_SEC);

	char* test2 = "define_matrix";
	start = clock();
	define_matrix(matrix, entries, len);
	end = clock();
	print_test(test2, test_definition(matrix, entries, len), (double) (end - start) / CLOCKS_PER_SEC);
	
	char* test3 = "copy_matrix";
	start = clock();
	Matrix* copy = copy_matrix(matrix);
	end = clock();
	print_test(test3, equal_matrix(matrix,copy,EPSILON), (double) (end - start) / CLOCKS_PER_SEC);

	char* test4 = "swap_rows";
	int a = rand()%copy->rows, b = (a + 1)%copy->rows;
	start = clock();
	swap_rows(copy, a, b);
	end = clock();
	print_test(test4, test_swap(matrix, copy, a, b, 1), (double) (end - start) / CLOCKS_PER_SEC);	
	swap_rows(copy, a, b);

	char* test5 = "swap_cols";
	a = rand()%copy->cols;
	b = (a + 1)%copy->cols;
	start = clock();
	swap_cols(copy, a, b);
	end = clock();
	print_test(test5, test_swap(matrix, copy, a, b, 0), (double) (end - start) / CLOCKS_PER_SEC);
	swap_cols(copy, a, b);

	char* test6 = "free_matrix";
	start = clock();
	free_matrix(copy);
	end = clock();
	print_test(test6, 1, (double) (end - start) / CLOCKS_PER_SEC);

	// char* test7 = "null_matrix";
	// int c = rand()%10 + 1, d = rand()%10 + 1;
	// Matrix* mat1 = null_matrix(c,d);
	// Matrix* mat2 = create_matrix(c,d);
	// print_test(test7, equal_matrix(mat1, mat2, EPSILON), -1);
	// free_matrix(mat1);
	// free_matrix(mat2);

	char* test8 = "id_matrix";
	int k = rand()%5 + 2;
	start = clock();
	Matrix* mat1 = id_matrix(k);
	end = clock();

	char* test9 = "mult_matrix";
	int start1, end1;
	start1 = clock();
	Matrix* mat2 = mult_matrix(mat1,mat1);
	end1 = clock();

	if (mat2 == NULL) value_err(NULL);
	else {
		print_test(test8, equal_matrix(mat1, mat2, EPSILON), (double) (end - start) / CLOCKS_PER_SEC);
		print_test(test9, equal_matrix(mat1, mat2, EPSILON),  (double) (end1 - start1) / CLOCKS_PER_SEC);
	}
	free_matrix(mat1);
	free_matrix(mat2);

	char* test10 = "is_id_matrix";
	start = clock();
	int test = is_id_matrix(mat1, EPSILON);
	end = clock();
	print_test(test10, test, (double) (end - start) / CLOCKS_PER_SEC);

	char* test11 = "is_null_matrix";
	start = clock();
	mat1 = null_matrix(k,k);
	test = is_null_matrix(mat1, EPSILON);
	end = clock();
	free_matrix(mat1);
	print_test(test11, test, (double) (end - start) / CLOCKS_PER_SEC);
}

int main() {
	srand(time(NULL));
	double entries[4] = {1,2,3,4};
	test_matrix(entries,4,2,2);
	return 0;
}
