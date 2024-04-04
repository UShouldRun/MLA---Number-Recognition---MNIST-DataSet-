// There is still more testing to do...

#include "matrix.h"
#include <time.h>

int test_definition(Matrix* matrix, double entries[], int len) {
	for (int i = 0; i < matrix->rows; i++)
		for (int j = 0; j < matrix->cols; j++)
			if (entries[i*matrix->cols + j] != matrix->data[i][j]) return 0;
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
	printf(" - CPU time: %.7lf\n", delta_time);	

	printf("\033[0m"); // Reset color
}

int is_prime(int n) {
	for (int i = 2; i*i <= n; i++) if (!(n%i)) return 0;
	return 1;
}

int get_factors(int n, int factors[]) {
	int len = 1;
	factors[0] = 1;
	for (int i = 2; i*i < n; i++)
		if (!(n%i)) {
			factors[len] = i;
			factors[len + 1] = n%i;
			len += 2;
		}
	factors[len + 1] = n;
	return len + 1;
}

void test() {
	clock_t start, end;

	char* test1 = "create_matrix / null_matrix";
	int n = rand()%10 + 1, m, len;
	
	while (is_prime(n)) n = rand()%10 + 1;
	int facts[n];
	for (int i = 0; i < n; i++) facts[i] = 0;
	len = get_factors(n, facts);
	do m = facts[rand()%(len-1)]; while (!m);
	len = n*m;

	double entries[len];
	for (int i = 0; i < len; i++) entries[i] = (double) (rand()%100);

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

	char* test8 = "id_matrix";
	int k = rand()%5 + 2;
	start = clock();
	Matrix* mat1 = id_matrix(k);
	end = clock();

	char* test9 = "mult_matrix";
	clock_t start1, end1;
	start1 = clock();
	Matrix* mat2 = mult_matrix(mat1,mat1);
	end1 = clock();

	if (mat2 == NULL) value_err(NULL);
	else {
		print_test(test8, equal_matrix(mat1, mat2, EPSILON), (double) (end - start) / CLOCKS_PER_SEC);
		print_test(test9, equal_matrix(mat1, mat2, EPSILON),  (double) (end1 - start1) / CLOCKS_PER_SEC);
	}

	char* test10 = "is_id_matrix";
	start = clock();
	int test = is_id_matrix(mat1, EPSILON);
	end = clock();
	print_test(test10, test, (double) (end - start) / CLOCKS_PER_SEC);
	free_matrix(mat1);

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
	test();
	return 0;
}
