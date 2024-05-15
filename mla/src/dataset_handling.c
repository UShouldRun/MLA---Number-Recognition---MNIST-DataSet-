#include "../include/dataset_handling.h"

void read_dataset(const char* csv_filename, const int training_samples, Vector* pixels[], Vector* labels[]) {
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    char *token;
    int num_samples = 0;

    // Open the file
    fp = fopen(csv_filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Read the file line by line
    while (fgets(line, sizeof(line), fp) != NULL && num_samples < training_samples) {
        // Tokenize the line
        token = strtok(line, ",");
        if (token == NULL) {
            fprintf(stderr, "Error: Empty line encountered.\n");
            continue;
        }

        // Read label
        labels[num_samples] = null_vector(MAX_LINE_LENGTH - 1);
        labels[num_samples]->data[atoi(token) - 1] = 1;

        // Allocate memory for pixel vector
        Vector* pixels_instance = create_vector(MAX_LINE_LENGTH - 1);

        // Read pixel values
        int pixel_count = 0;
        while ((token = strtok(NULL, ",")) != NULL && pixel_count < MAX_LINE_LENGTH - 1) {
            pixels_instance->data[pixel_count] = atoi(token);
            pixel_count++;
        }

        // Store pixel vector
        pixels[num_samples] = pixels_instance;

        // Increment sample count
        num_samples++;
    }

    // Close the file
    fclose(fp);
}

void write_data(const char* file, const int length, Matrix* edges[], Vector* biases[]) {
    FILE *fp = fopen(file, "w");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Write matrix data to file
    for (int i = 0; i < length; i++) {
        for (int row = 0; row < edges[i]->rows; row++) {
            for (int col = 0; col < edges[i]->cols; col++) {
                fprintf(fp, "%lf,", edges[i]->data[row][col]);
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n"); // Separate matrices with a blank line
    }

    // Write vector biases to file
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < biases[i]->len; j++) {
            fprintf(fp, "%lf,", biases[i]->data[j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void read_data(const char* file, const int length, Matrix* edges[], Vector* biases[]) {
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Read matrix data from file
    for (int i = 0; i < length; i++) {
        for (int row = 0; row < edges[i]->rows; row++) {
            for (int col = 0; col < edges[i]->cols; col++) {
                if (fscanf(fp, "%lf,", &edges[i]->data[row][col]) != 1) {
                    fprintf(stderr, "Error reading matrix data from file\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
        // Read blank line separator
        char blankLine[100];
        fgets(blankLine, sizeof(blankLine), fp);
    }

    // Read vector biases from file
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < biases[i]->len; j++) {
            if (fscanf(fp, "%lf,", &biases[i]->data[j]) != 1) {
                fprintf(stderr, "Error reading vector data from file\n");
                exit(EXIT_FAILURE);
            }
        }
        // Read newline character separator
        char newLine;
        fscanf(fp, "%c", &newLine);
    }

    fclose(fp);
}


