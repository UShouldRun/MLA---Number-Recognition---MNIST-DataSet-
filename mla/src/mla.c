#include "../include/mla.h"

void update_progress_bar(long i, double cost) {
    const int bar_width = 50;
    float completion_percentage = ((float) (i * GUESS_STACK + 1) / TRAINING_SAMPLES) * 100;
    int completed_width = (completion_percentage * bar_width) / 100;

    printf("\033[38;5;208m");
    printf("\r");
    printf("Training Section: Stack %ld - Cost %lf [", i, cost);
    for (int i = 0; i < completed_width; i++) printf("=");
    for (int i = completed_width; i < bar_width; i++) printf(" ");
    printf("] %.1f%% ", completion_percentage);
    fflush(stdout);
    printf("\033[0m");
}

void displayEdge(LogFile* file, void* object) {
    Matrix* edge = (Matrix*)object;
    fprintf(file->file, "Matrix* edge : adress - %p : rows - %ld : cols - %ld\n", (void*)edge, edge->rows, edge->cols);
    fprintf(file->file, "Data:\n");
    for (size_t i = 0; i < edge->rows; i++) {
        fprintf(file->file, (i == 0 || i == edge->rows - 1) ? "(" : "|");
        if (edge->rows > 16 && i == 8) {
            fprintf(file->file, " ... ");
            i += edge->rows - 16;
            continue;
        }
        for (size_t j = 0; j < edge->cols; j++) {
            if (edge->cols > 16 && j == 8) {
                fprintf(file->file, " ... ");
                j += edge->cols - 16;
                continue;
            }
            fprintf(file->file, " %.2lf ", edge->data[i][j]);
            if (j == edge->cols - 1) fprintf(file->file, (i == 0 || i == edge->rows - 1) ? ")\n" : "|\n");
        }
    }
}

void displayNode(LogFile* file, void* object) {
    Node* node = (Node*)object;
    fprintf(file->file, "Node* node : adress - %p : id - %ld : visits - %ld : reward - %ld : state->len - %ld\n", (void*)node, node->id, node->visits, node->reward, node->state->len);
    fprintf(file->file, "State data:\n");
    fprintf(file->file, "(");
    for (size_t i = 0; i < node->state->len; i++)
         fprintf(file->file, " %.2lf ", node->state->data[i]);
    fprintf(file->file, ")\n");
}

void displayBias(LogFile* file, void* object) {
    Vector* bias = (Vector*)object;
    fprintf(file->file, "Vector* bias : adress - %p : len - %ld\n", (void*)bias, bias->len);
    fprintf(file->file, "Data:\n");
    fprintf(file->file, "(");
    for (size_t i = 0; i < bias->len; i++)
         fprintf(file->file, " %.2lf ", bias->data[i]);
    fprintf(file->file, ")\n");
}

void displayAll(LogFile* file, LogEntry* entry, Matrix* edges[EDGES], Vector* biases[EDGES]) {
    for (long i = 0; i < EDGES; i++) {
        writeObjectData(file, entry, edges[i], displayEdge);
        writeObjectData(file, entry, biases[i], displayBias);
    }
}

void create_and_train_mla(LogFile* logFile, const char* data_mla, Vector* input_pixels[TRAINING_SAMPLES], Vector* labels[TRAINING_SAMPLES]) {
    LogEntry* logEntry = (LogEntry*)malloc(sizeof(LogEntry));
    initializeTask(&logEntry->task, logFile->taskCounter++, "void create_and_train_mla()");

    void writeTaskStatus(const char* status) { writeLogTaskStatus(logFile, logEntry, status); }

    srand(time(NULL));

    Vector* bias1 = create_vector(NODE_LEVEL_1);
    double random_bias1[NODE_LEVEL_1];
    for (long i = 0; i < NODE_LEVEL_1; i++) random_bias1[i] = rand() % MAX_ENTRY;
    define_vector(bias1, random_bias1, NODE_LEVEL_1);
    writeTaskStatus("created bias1");
    Vector* bias2 = create_vector(NODE_LEVEL_2);
    double random_bias2[NODE_LEVEL_2];
    for (long i = 0; i < NODE_LEVEL_2; i++) random_bias2[i] = rand() % MAX_ENTRY;
    define_vector(bias2, random_bias2, NODE_LEVEL_2);
    writeTaskStatus("created bias2");
    Vector* output_bias = null_vector(NODE_LEVEL_3);
    double random_bias3[NODE_LEVEL_3];
    for (long i = 0; i < NODE_LEVEL_3; i++) random_bias3[i] = rand() % MAX_ENTRY;
    define_vector(output_bias, random_bias3, NODE_LEVEL_3);
    writeTaskStatus("created output_bias");
    Vector* biases[EDGES] = { bias1, bias2, output_bias };

    Node* picture_node = create_node(0, 0, NULL);
    writeTaskStatus("created picture_node");
    Node* layer1_node = create_node(1, 0, NULL);
    writeTaskStatus("created layer1_node");
    Node* layer2_node = create_node(2, 0, NULL);
    writeTaskStatus("created layer2_node");
    Node* output_node = create_node(3, 0, NULL);
    writeTaskStatus("created output_node");
    Node* nodes[NODES] = { picture_node, layer1_node, layer2_node, output_node };

    nodes[1]->state = null_vector(NODE_LEVEL_1);
    writeTaskStatus("created new nodes[1]->state instance -> creating new nodes[2]->state instance");
    nodes[2]->state = null_vector(NODE_LEVEL_2);
    writeTaskStatus("created new nodes[2]->state instance -> creating new nodes[3]->state instance");
    nodes[3]->state = null_vector(NODE_LEVEL_3);
    writeTaskStatus("created new nodes[3]->state instance -> network relationship()");

    picture_node->children[0] = 1;
    layer1_node->parents[0] = 0;
    layer1_node->children[0] = 2;
    layer2_node->parents[0] = 1;
    layer2_node->children[0] = 3;
    output_node->parents[0] = 2;
    writeTaskStatus("created network relationship");

    Matrix* edge1 = create_matrix(NODE_LEVEL_1, PIXELS);
    double random_entries1[NODE_LEVEL_1 * PIXELS];
    for (long i = 0; i < NODE_LEVEL_1 * PIXELS; i++) random_entries1[i] = rand() % MAX_ENTRY;
    define_matrix(edge1, random_entries1, NODE_LEVEL_1 * PIXELS);
    writeTaskStatus("created edge1");

    Matrix* edge2 = create_matrix(NODE_LEVEL_2, NODE_LEVEL_1);
    double random_entries2[NODE_LEVEL_2 * NODE_LEVEL_1];
    for (long i = 0; i < NODE_LEVEL_2 * NODE_LEVEL_1; i++) random_entries2[i] = rand() % MAX_ENTRY;
    define_matrix(edge2, random_entries2, NODE_LEVEL_2 * NODE_LEVEL_1);
    writeTaskStatus("created edge2");

    Matrix* edge3 = create_matrix(NODE_LEVEL_3, NODE_LEVEL_2);
    double random_entries3[NODE_LEVEL_3 * NODE_LEVEL_2];
    for (long i = 0; i < NODE_LEVEL_3 * NODE_LEVEL_2; i++) random_entries3[i] = rand() % MAX_ENTRY;
    define_matrix(edge3, random_entries3, NODE_LEVEL_3 * NODE_LEVEL_2);
    writeTaskStatus("created edge3");

    Matrix* edges[EDGES] = { edge1, edge2, edge3 };

    Vector* guess[GUESS_STACK];
    Node* stored_network[GUESS_STACK][NODES];

    writeTaskStatus("initializing training");
    for (long i = 0; i < TRAINING_SAMPLES / GUESS_STACK; i++) {
        writeTaskStatus("displaying network");
        displayAll(logFile, logEntry, edges, biases);

        for (long j = 0; j < GUESS_STACK; j++) {
            writeTaskStatus("loading input pixels...");
            nodes[0]->state = input_pixels[i * TRAINING_SAMPLES + j];
            writeObjectData(logFile, logEntry, nodes[0], displayNode);
            writeTaskStatus("loaded input pixels -> cleaning network");
            for (long k = 1; k < NODES; k++)
                for (int l = 0; l < nodes[k]->state->len; l++)
                    nodes[k]->state->data[l] = 0;
            writeTaskStatus("cleaned network -> network_guess");

            int errorCode;
            if ((errorCode = network_guess(nodes, edges, biases)) != 0) {
                logError(logFile, &logEntry->task, errorCode, "error in nla.c functions or network_guess() internal structure", "mla/src/nn_functions.c : in function int network_guess() : 157");
                setLogLevel(logEntry, "FATAL");
                strncpy(logEntry->message, logEntry->task.lastError.errorMessage, sizeof(logEntry->message) - 1);
                writeLogEntry(logFile, logEntry);
                fclose(logFile->file);
                exit(logEntry->task.lastError.errorCode);
            }
            writeTaskStatus("network_guess() finished -> storing data");

            guess[j] = copy_vector(nodes[3]->state);
            for (long k = 0; k < NODES; k++) stored_network[j][k] = copy_node(nodes[k]);
            writeTaskStatus("stored data -> freeing memory");
        }
        writeTaskStatus("loading label...");
        Vector* expected[GUESS_STACK];
        for (int k = 0; k < GUESS_STACK; k++) expected[k] = copy_vector(labels[i * GUESS_STACK + k]);
        writeTaskStatus("loaded label -> gradient_descent()");

        update_progress_bar(i, cost(expected, guess));
        int errorCode;
        if ((errorCode = gradient_descent(expected, guess, edges, biases, stored_network)) != 0) {
            logError(logFile, &logEntry->task, errorCode, "error in nn_functions.c functions or gradient_descent() internal structure", "mla/src/nn_functions.c : in function int gradient_descent() : 178");
            setLogLevel(logEntry, "FATAL");
            strncpy(logEntry->message, logEntry->task.lastError.errorMessage, sizeof(logEntry->message) - 1);
            writeLogEntry(logFile, logEntry);
            fclose(logFile->file);
            exit(logEntry->task.lastError.errorCode);
        }
        writeTaskStatus("gradient_descent() complete -> freeing stored network");

        for (long j = 0; j < GUESS_STACK; j++) {
            for (long k = 0; k < NODES; k++)
                free_node(stored_network[j][k]);
            free_vector(guess[j]);
        }
    }

    writeTaskStatus("training complete -> write_data()");
    write_data(logFile, data_mla, TRAINING_SAMPLES, edges, biases);
    writeTaskStatus("write_data() finished -> freeing allocated memory...");

    for (int i = 0; i < NODES; i++) free_node(nodes[i]);
    for (int i = 0; i < EDGES; i++) { free_matrix(edges[i]); free_vector(biases[i]); }
    writeTaskStatus("task complete");
}
