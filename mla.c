#include "math/node.h"
#include "math/nla.h"
#include <time.h>

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

int read_data_set(Vector* pixels, char data_set[], long file) { 
    FILE *fp;
    unsigned char *buf;
    int magic_number, num_items, num_rows, num_cols;

    // Open the file
    fp = fopen("train-images-idx3-ubyte", "rb");

    // Read the magic number
    fread(&magic_number, sizeof(magic_number), 1, fp);
    magic_number = __builtin_bswap32(magic_number);

    // Read the number of items
    fread(&num_items, sizeof(num_items), 1, fp);
    num_items = __builtin_bswap32(num_items);

    // Read the number of rows
    fread(&num_rows, sizeof(num_rows), 1, fp);
    num_rows = __builtin_bswap32(num_rows);

    // Read the number of columns
    fread(&num_cols, sizeof(num_cols), 1, fp);
    num_cols = __builtin_bswap32(num_cols);

    printf("Magic number: %d\n", magic_number);
    printf("Number of items: %d\n", num_items);
    printf("Number of rows: %d\n", num_rows);
    printf("Number of columns: %d\n", num_cols);

    // Allocate memory to store image data
    buf = (unsigned char *)malloc(num_rows * num_cols * sizeof(unsigned char));

    // Read image data
    fread(buf, sizeof(unsigned char), num_rows * num_cols, fp);

    // Close the file
    fclose(fp);

    // Free allocated memory
    free(buf);

    return 0;
}

void network_guess(Node* nodes[NODES], Matrix* edges[EDGES], Vector* biases[EDGES]) {
    Vector* raw_layer1 = matrix_vector_mul(edges[0], nodes[0]->state);
    for (long i = 0; i < raw_layer1->len; i++) nodes[1]->state->data[i] = sigmoid(raw_layer1->data[i] + biases[0]->data[i]);

    Vector* raw_layer2 = matrix_vector_mul(edges[1], nodes[1]->state);
    for (long i = 0; i < raw_layer2->len; i++) nodes[2]->state->data[i] = sigmoid(raw_layer2->data[i] + biases[1]->data[i]);

    Vector* raw_output = matrix_vector_mul(edges[2], nodes[2]->state);
    for (long i = 0; i < raw_output->len; i++) nodes[3]->state->data[i] = sigmoid(raw_output->data[i] + biases[2]->data[i]);
}

double cost(Vector* expected[GUESS_STACK], Vector* guess[GUESS_STACK], long entry) {
    double sum = 0;
    for (long n; n < GUESS_STACK; n++)
        for (int i = 0; i < NODE_LEVEL_3; i++)
            sum += pow_int(expected[n]->data[i] - guess[n]->data[i], 2);
    return sum/(2*GUESS_STACK);
}

double sigmoid_partial_guess(double impulse) { return - pow_int(sigmoid(impulse), 2) * exp(-impulse); }

double impulse_partial_weight(int x, ID* id, Matrix* weights[EDGES], Node* nodes[GUESS_STACK][NODES]) {
    double recursive(int layer, long row, ID* id, Matrix* weights[EDGES], Node* nodes[GUESS_STACK][NODES]) {
        double sum = 0;
        for (long j = 0; j < weights[0]->cols; j++) { 
            if (layer < id->layer) sum += recursive(layer - 1, 0, id, weights, nodes) * weights[layer]->data[row][j];
            sum += (layer == id->layer && row == id->i && j == id->j) ? nodes[x][layer-1]->state->data[row] : 0;
        }
        return sum;
    }
    return recursive(EDGES, 0, id, weights, nodes);
}

double impulse_partial_bias(ID* id, Matrix* weights[EDGES], Vector* biases[EDGES]) {
    double recursive(int layer, long row, ID* id, Matrix* weights[EDGES], Vector* biases[EDGES]) {
        if (layer == id->layer && row == id->i) return 1;
        double sum = 0;
        for (long j = 0; j < weights[0]->cols && layer < id->layer; j++)
            sum += recursive(layer - 1, 0, id, weights, biases) * weights[layer]->data[row][j];
        return sum;
    }
    return recursive(EDGES, 0, id, weights, biases);
}

double impulse_partial_var(int x, ID* id, Matrix* weights[EDGES], Vector* biases[EDGES], Node* nodes[GUESS_STACK][NODES]) {
    return id->flag ? impulse_partial_bias(id, weights, biases) : impulse_partial_weight(x, id, weights, nodes);
}

double cost_partial_var(Vector* expected[GUESS_STACK], Vector* guess[GUESS_STACK], ID* id, Matrix* weights[EDGES], Vector* biases[EDGES], Node* nodes[GUESS_STACK][NODES]) {
    double step = 0;
    for (int x = 0; x < GUESS_STACK; x++)
        for (int i = 0; i < guess[0]->len; i++)
            step += (guess[x]->data[i] - expected[x]->data[i]) * sigmoid_partial_guess(guess[x]->data[i]) * impulse_partial_var(x, id, weights, biases, nodes);
    return -step/GUESS_STACK;
}

void gradient_descent(Vector* expected[GUESS_STACK], Vector* guess[GUESS_STACK], Matrix* weights[EDGES], Vector* biases[EDGES], Node* nodes[GUESS_STACK][NODES]) {
    ID* id = malloc(sizeof(ID));
    assert(id == NULL);
    for (int k = EDGES - 1; k > -1; k--)
        for (long i = 0; i < weights[k]->rows; i++) {
            id->flag = 0; id->layer = k; id->i = i;
            for (long j = 0; j < weights[k]->cols; j++) {
                id->j = j;
                weights[k]->data[i][j] += cost_partial_var(expected, guess, id, weights, biases, nodes);
            }
            id->flag = 1;
            biases[k]->data[i] += cost_partial_var(expected, guess, id, weights, biases, nodes);
        }
    free(id);
}

void write_data(Matrix* edges[EDGES], Vector* biased[EDGES], char file[]) { return; }

int create_and_train_mla(char data_set[], char data_mla[]) {
    srand(time(NULL));

    Vector* pixels = null_vector(PIXELS);
    Vector* layer1 = null_vector(NODE_LEVEL_1);
    Vector* layer2 = null_vector(NODE_LEVEL_2);
    Vector* layer3 = null_vector(NODE_LEVEL_3);

    Vector* bias1 = null_vector(NODE_LEVEL_1);
    Vector* bias2 = null_vector(NODE_LEVEL_2);
    Vector* output_bias = null_vector(NODE_LEVEL_3);
    Vector* biases[EDGES] = { bias1, bias2, output_bias };

    Node* picture_node = create_node(0, 0, pixels);
    Node* layer1_node = create_node(1, 0, layer1);
    Node* layer2_node = create_node(2, 0, layer2);
    Node* output_node = create_node(3, 0, layer3);
    Node* nodes[NODES] = { picture_node, layer1_node, layer2_node, output_node };

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

    Matrix* edges[EDGES] = { edge1, edge2, edge3 };

    int numbers[GUESS_STACK];
    Vector* guess[GUESS_STACK];
    Node* stored_network[GUESS_STACK][NODES];

    for (long i = 0; i < TRAINING_SAMPLES / GUESS_STACK; i++) {
        for (long j = 0; j < GUESS_STACK; j++) {
            nodes[1]->state = null_vector(NODE_LEVEL_1);
            nodes[2]->state = null_vector(NODE_LEVEL_2);
            nodes[3]->state = null_vector(NODE_LEVEL_3);

            numbers[j] = read_data_set(picture_node->state, data_set, i * GUESS_STACK + j);
            network_guess(nodes, edges, biases);
            guess[j] = copy_vector(nodes[3]->state);

            for (int k = 0; k < NODES; k++) stored_network[j][k] = nodes[k];
        }
        Vector* expected[GUESS_STACK];
        for (int i = 0; i < GUESS_STACK; i++) {
            expected[i] = null_vector(NODE_LEVEL_3);
            for (int j = 0; j < NODE_LEVEL_3; j++)
                expected[i]->data[j] = j == numbers[j] - 1;
        }
        gradient_descent(expected, guess, edges, biases, stored_network);
    }

    write_data(edges, biases, data_mla);

    return EXIT_SUCCESS;
}

int main() { return EXIT_SUCCESS; }
