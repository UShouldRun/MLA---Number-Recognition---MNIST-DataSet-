<<<<<<< HEAD
#ifndef NODE_H
#define NODE_H

=======
#include <stlib.h>
>>>>>>> origin/main
#include <assert.h>
#include "nla.h"

#define MAX_PARENTS 100
#define MAX_CHILDREN 100

<<<<<<< HEAD
typedef struct {
=======
type struct {
>>>>>>> origin/main
    long id;
    long parents[MAX_PARENTS];
    long children[MAX_CHILDREN];
    long visits;
    long reward;
    Vector* state;
} Node;

Node* create_node(long id, long reward, Vector* state);

void free_node(Node* node);
void set_parents(Node* node, long parents[], size_t length);
void set_children(Node* node, long children[], size_t length);

int get_degree(Node* node);
<<<<<<< HEAD

#endif
=======
>>>>>>> origin/main
