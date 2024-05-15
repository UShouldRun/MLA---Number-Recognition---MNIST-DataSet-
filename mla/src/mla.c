#include "../include/mla.h"

void update_progress_bar(int progress) {
    const int bar_width = 50;
    float completion_percentage = ((float) (progress + 1) / TRAINING_SAMPLES) * 100;
    int completed_width = (completion_percentage * bar_width) / 100;

    printf("\033[38;5;208m");
    printf("\r[");
    for (int i = 0; i < completed_width; i++) {
        printf("=");
    }
    for (int i = completed_width; i < bar_width; i++) {
        printf(" ");
    }
    printf("] %.1f%% ", completion_percentage);
    fflush(stdout); // Flush the output to ensure it's displayed immediately
    printf("\033[0m");
}

void create_and_train_mla(const char* data_mla, Vector* input_pixels[TRAINING_SAMPLES], Vector* labels[TRAINING_SAMPLES]) {
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
            nodes[0]->state = input_pixels[i * TRAINING_SAMPLES + j];
            nodes[1]->state = null_vector(NODE_LEVEL_1);
            nodes[2]->state = null_vector(NODE_LEVEL_2);
            nodes[3]->state = null_vector(NODE_LEVEL_3);

            network_guess(nodes, edges, biases);
            guess[j] = copy_vector(nodes[3]->state);

            for (int k = 0; k < NODES; k++) stored_network[j][k] = nodes[k];
        }
        Vector* expected[GUESS_STACK];
        for (int k = 0; k < GUESS_STACK; k++) expected[k] = copy_vector(labels[i * GUESS_STACK + k]);

        printf("\r[");
        printf("Training Section: Stack %ld - Cost %lf\n", i, cost(expected, guess));
        fflush(stdout);
        update_progress_bar(i * GUESS_STACK);

        gradient_descent(expected, guess, edges, biases, stored_network);
    }

    write_data(data_mla, TRAINING_SAMPLES, edges, biases);

    for (int i = 0; i < NODES; i++) free_node(nodes[i]);
    for (int i = 0; i < EDGES; i++) { free_matrix(edges[i]); free_vector(biases[i]); }
}
