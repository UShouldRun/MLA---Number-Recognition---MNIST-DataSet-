#include "../include/node.h"

Node* create_node(long id, long reward, Vector* state) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) node_mem_err(NULL, 4);
    node->id = id;
    for (long i = 0; i < MAX_PARENTS; i++) node->parents[i] = -1;
    for (long i = 0; i < MAX_CHILDREN; i++) node->children[i] = -1;
    node->reward = reward;
    node->state = state;
    return node;
}

Node* copy_node(Node* node) {
    return create_node(node->id, node->reward, copy_vector(node->state));
}

void node_mem_err(void *ptr, int line) {
     if (ptr == NULL) {
          printf("Runtime error : math/src/node.c : %d : memory allocation failed", line);
          exit(EXIT_FAILURE);
     }
}

void node_value_err(void *ptr, int line) {
     if (ptr == NULL) {
          printf("Runtime error : math/src/node.c : %d : Value error\n", line);
          exit(EXIT_FAILURE);
     }
}

void free_node(Node* node) {
    if (node == NULL) return;
    free_vector(node->state);
    free(node);
}

void set_parents(Node* node, long parents[], size_t length) {
    if (length > MAX_PARENTS) node_value_err(NULL, 34);
    for (size_t i = 0; i < length; i++) node->parents[i] = parents[i];
}

void set_children(Node* node, long children[], size_t length) {
    if (length > MAX_CHILDREN) node_value_err(NULL, 39);
    for (size_t i = 0; i < length; i++) node->children[i] = children[i];
}

int get_degree(Node* node) {
    long i, j;
    for (; node->children[i] != -1; i++);
    for (; node->parents[j] != -1; j++);
    return i + j;
}
