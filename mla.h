#ifndef MLA_H
#define MLA_H

#include "math/node.h"
#include "math/nla.h"
#include <time.h>
#include <errno.h>

#define EXIT_SUCCESS 0
#define MALLOC_FAIL 1

#define SAMPLES 70000
#define TEST_SAMPLES SAMPLES * 1/7
#define TRAINING_SAMPLES SAMPLES * 6/7

#define GUESS_STACK 10
#define LEARNING_FACTOR 1

#define NODES 4
#define EDGES 3

#define PIXELS 728
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

void read_data_set(char data_set[], Vector* pixels[TRAINING_SAMPLES]);
void create_and_train_mla(char data_mla[], Vector* input_pixels[TRAINING_SAMPLES], Vector* labels[TRAINING_SAMPLES]);
void write_data(Matrix* edges[EDGES], Vector* biased[EDGES], char file[]);
void read_data(Matrix* edges[EDGES], Vector* biases[EDGES], char file[]);

void update_progress_bar(int progress);
void network_guess(Node* nodes[NODES], Matrix* edges[EDGES], Vector* biases[EDGES]);
void gradient_descent(Vector* expected[GUESS_STACK], Vector* guess[GUESS_STACK], Matrix* weights[EDGES], Vector* biases[EDGES], Node* nodes[GUESS_STACK][NODES]);

double cost(Vector* expected[GUESS_STACK], Vector* guess[GUESS_STACK]);
double sigmoid_partial_guess(double impulse);
double impulse_partial_weight(int x, ID* id, Matrix* weights[EDGES], Node* nodes[GUESS_STACK][NODES]);
double impulse_partial_bias(ID* id, Matrix* weights[EDGES], Vector* biases[EDGES]);
double impulse_partial_var(int x, ID* id, Matrix* weights[EDGES], Vector* biases[EDGES], Node* nodes[GUESS_STACK][NODES]);
double cost_partial_var(Vector* expected[GUESS_STACK], Vector* guess[GUESS_STACK], ID* id, Matrix* weights[EDGES], Vector* biases[EDGES], Node* nodes[GUESS_STACK][NODES]);

#endif
