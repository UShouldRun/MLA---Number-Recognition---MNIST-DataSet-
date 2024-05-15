#include "../include/nn_functions.h"

double cost(Vector* expected[GUESS_STACK], Vector* guess[GUESS_STACK]) {
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
    return - LEARNING_FACTOR * step/GUESS_STACK;
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

void network_guess(Node* nodes[NODES], Matrix* edges[EDGES], Vector* biases[EDGES]) {
    Vector* raw_layer1 = matrix_vector_mul(edges[0], nodes[0]->state);
    for (long i = 0; i < raw_layer1->len; i++) nodes[1]->state->data[i] = sigmoid(raw_layer1->data[i] + biases[0]->data[i]);

    Vector* raw_layer2 = matrix_vector_mul(edges[1], nodes[1]->state);
    for (long i = 0; i < raw_layer2->len; i++) nodes[2]->state->data[i] = sigmoid(raw_layer2->data[i] + biases[1]->data[i]);

    Vector* raw_output = matrix_vector_mul(edges[2], nodes[2]->state);
    for (long i = 0; i < raw_output->len; i++) nodes[3]->state->data[i] = sigmoid(raw_output->data[i] + biases[2]->data[i]);
}
