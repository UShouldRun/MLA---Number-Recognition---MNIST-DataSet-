#ifndef DATASET_HANDLING_H
#define DATASET_HANDLING_H

#include "../../math/include/nla.h"
#include <errno.h>
#include <string.h>

#define MAX_LINE_LENGTH 785  // For MNIST, each image has 28x28 pixels + 1 label

void read_dataset(const char* csv_filename, const int training_samples, Vector* pixels[], Vector* labels[]);

void read_data(const char* cvs_file, const int length, Matrix* edges[], Vector* biases[]);
void write_data(const char* cvs_file, const int length, Matrix* edges[], Vector* biased[]);

#endif
