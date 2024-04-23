#include "node.h"
#include "matrix.h"
#include <time.h>

#define TRAINING_SAMPLES 60000
#define GUESS_STACK 10

#define NODES 4
#define EDGES 3

#define PIXELS 728
#define NODE_LEVEL_1 16
#define NODE_LEVEL_2 16
#define NODE_LEVEL_3 10

#define MAX_ENTRY 100

int read_data_set(Vector* pixels, char data_set[], long file) { return 0; }

void guess(Node* nodes[NODES], Matrix* edges[EDGES], Vector* biases[EDGES]) {
    Vector* raw_layer1 = matrix_vector_mul(edges[0], nodes[0]->state);
    for (long i = 0; i < raw_layer1->len; i++) nodes[1]->state->data[i] = sigmoid(raw_layer1->data[i] + biases[0]->data[i]);

    Vector* raw_layer2 = matrix_vector_mul(edges[1], nodes[1]->state);
    for (long i = 0; i < raw_layer2->len; i++) nodes[2]->state->data[i] = sigmoid(raw_layer2->data[i] + biases[1]->data[i]);

    Vector* raw_output = matrix_vector_mul(edges[2], nodes[2]->state);
    for (long i = 0; i < raw_output->len; i++) nodes[3]->state->data[i] = sigmoid(raw_output->data[i] + biases[2]->data[i]);
}

double cost(Vector* expected[GUESS_STACK], Vector* guesses[GUESS_STACK]) {
    double sum = 0;
    long n = 0;
    for (; n < GUESS_STACK; n++) sum += pow_int(expected[n] - guesses[n], 2)
    return sum/(2*n);
}

void feedback(Matrix* edges[EDGES], Vector* biases[EDGES], int numbers[GUESS_STACK], Vector* guesses[GUESS_STACK]) {
    return;
}

void write_data(Matrix* edges, Vector* biased[NODES], char file[]) { return; }

int create_and_train_mla(char data_set[], char data_mla[]) {
    srand(time(NULL));

    Vector* pixels = null_vector(PIXELS);
    Vector* layer1 = null_vector(NODE_LEVEL_1);
    Vector* layer2 = null_vector(NODE_LEVEL_2);
    Vector* layer3 = null_vector(NODE_LEVEL_3);

    Vector* bias1 = null_vector(NODE_LEVEL_1);
    Vector* bias2 = null_vector(NODE_LEVEL_2);
    Vector* output_bias = null_vector(NODE_LEVEL_3);
    Vector* biases[EDGES] = { &bias1, &bias2, &output_bias };

    Node* picture_node = create_node(0, 0, pixels);
    Node* layer1_node = create_node(1, 0, layer1);
    Node* layer2_node = create_node(2, 0, layer2);
    Node* output_node = create_node(3, 0, layer3);
    Node* nodes[NODES] = { &picture_node, &layer1_node, &layer2_node, &output_node };

    picture_node->children[0] = 1;
    layer1_node->parents[0] = 0;
    layer1_node->children[0] = 2;
    layer2_node->parents[0] = 1;
    layer2_node->children[0] = 3;
    output_node->parents[0] = 2;

    Matrix* edge1 = create_matrix(NODE_LEVEL_1, pixels->len);
    double random_entries1[NODE_LEVEL_1 * pixels->len];
    for (long i = 0; i < NODE_LEVEL_1 * pixels->len; i++) random_entries1[i] = rand() % MAX_ENTRY;
    define_matrix(edge1, random_entries1, NODE_LEVEL_1 * pixels->len);

    Matrix* edge2 = create_matrix(NODE_LEVEL_2, NODE_LEVEL_1);
    double random_entries2[NODE_LEVEL_2 * NODE_LEVEL_1];
    for (long i = 0; i < NODE_LEVEL_2 * NODE_LEVEL_1; i++) random_entries2[i] = rand() % MAX_ENTRY;
    define_matrix(edge2, random_entries2, NODE_LEVEL_2 * NODE_LEVEL_1);

    Matrix* edge3 = create_matrix(NODE_LEVEL_3, NODE_LEVEL_2);
    double random_entries3[NODE_LEVEL_3 * NODE_LEVEL_2];
    for (long i = 0; i < NODE_LEVEL_3 * NODE_LEVEL_2; i++) random_entries3[i] = rand() % MAX_ENTRY;
    define_matrix(edge3, random_entries3, NODE_LEVEL_3 * NODE_LEVEL_2);

    Matrix* edges = { &edge1, &edge2, &edge3 };

    int numbers[GUESS_STACK];
    Vector* guesses[GUESS_STACK];

    for (long i = 0; i < TRAINING_SAMPLES / GUESS_STACK; i++) {
        for (long j = 0; j < GUESS_STACK; j++) {
            nodes[1]->state = null_vector(NODE_LEVEL_1);
            nodes[2]->state = null_vector(NODE_LEVEL_2);
            nodes[3]->state = null_vector(NODE_LEVEL_3);
            numbers[j] = read_data_set(picture_node->state, data_set, i * GUESS_STACK + j);
            guess(nodes, edges, biases);
            guesses[j] = copy_vector(nodes[3]->state);
        }
        feedback(edges, numbers, guesses);
    }

    write_data(edges, biases, data_mla);

    return 0;
}
