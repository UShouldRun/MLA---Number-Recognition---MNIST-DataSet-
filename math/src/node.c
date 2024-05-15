#include "../include/node.h"

Node* create_node(long id, long reward, Vector* state) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->id = id;
    for (long i = 0; i < MAX_PARENTS; i++) node->parents[i] = -1;
    for (long i = 0; i < MAX_CHILDREN; i++) node->children[i] = -1;
    node->reward = reward;
    node->state = state;
    return node;
}

void free_node(Node* node) {
    free_vector(node->state);
    free(node);
}

void set_parents(Node* node, long parents[], size_t length) {
    assert(length > MAX_PARENTS);
    for (size_t i = 0; i < length; i++) node->parents[i] = parents[i];
}

void set_children(Node* node, long children[], size_t length) {
    assert(length > MAX_CHILDREN);
    for (size_t i = 0; i < length; i++) node->children[i] = children[i];
}

int get_degree(Node* node) {
    long i, j;
    for (; node->children[i] != -1; i++);
    for (; node->parents[j] != -1; j++);
    return i + j;
}
