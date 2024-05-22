#include "../include/funcs.h"

long factorial(long n) {
    long fact = 1;
    for (long i = 1; i <= n; i++) fact *= i;
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
    return result;
}

double exp(double x) {
    if (x == 0) return 1;
    long double sum = 0, prev = 0;
    for (long n = 0; abs_d((sum += pow_int(x, n) / factorial(n)) - prev) > EPSILON; n++) prev = sum;
    return sum;
}

double sigmoid(double x) { return 1/(1 + exp(-x)); }

double abs_d(double number) { return number < 0 ? -number : number; } 

double sqrt_d(double number) {
    double x0 = number/2;
    double newtons_method(double x_n, int iteration) {
        if (iteration == 50) return x_n;
        return newtons_method(x_n/2 - x0/(2 * x_n), ++iteration);
    }
    return newtons_method(x0, 0);
}
