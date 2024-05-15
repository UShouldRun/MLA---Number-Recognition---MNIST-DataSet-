#ifndef NN_FUNCTIONS_H
#define NN_FUNCTIONS_H

#include "../../math/include/node.h"
#include "../../math/include/nla.h"

#define EXIT_SUCCESS 0
#define MALLOC_FAIL 1

#define SAMPLES 70000
#define TEST_SAMPLES SAMPLES * 1/7
#define TRAINING_SAMPLES SAMPLES * 6/7

#define GUESS_STACK 10
#define LEARNING_FACTOR 1

#define NODES 4
#define EDGES 3

#define PIXELS 784
#define NODE_LEVEL_1 16
#define NODE_LEVEL_2 16
#define NODE_LEVEL_3 10

#define MAX_ENTRY 100

typedef struct {
    short unsigned flag;
    long layer;
    long i;
    long j;
} ID;

double cost(Vector* expected[GUESS_STACK], Vector* guess[GUESS_STACK]);
double sigmoid_partial_guess(double impulse);
double impulse_partial_weight(int x, ID* id, Matrix* weights[EDGES], Node* nodes[GUESS_STACK][NODES]);
double impulse_partial_bias(ID* id, Matrix* weights[EDGES], Vector* biases[EDGES]);
double impulse_partial_var(int x, ID* id, Matrix* weights[EDGES], Vector* biases[EDGES], Node* nodes[GUESS_STACK][NODES]);
double cost_partial_var(Vector* expected[GUESS_STACK], Vector* guess[GUESS_STACK], ID* id, Matrix* weights[EDGES], Vector* biases[EDGES], Node* nodes[GUESS_STACK][NODES]);

void gradient_descent(Vector* expected[GUESS_STACK], Vector* guess[GUESS_STACK], Matrix* weights[EDGES], Vector* biases[EDGES], Node* nodes[GUESS_STACK][NODES]);
void network_guess(Node* nodes[NODES], Matrix* edges[EDGES], Vector* biases[EDGES]);

#endif
