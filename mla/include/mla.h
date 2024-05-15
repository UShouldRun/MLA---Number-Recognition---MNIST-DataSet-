#ifndef MLA_H
#define MLA_H

#include "nn_functions.h"
#include "dataset_handling.h"
#include <time.h>

void update_progress_bar(int progress);
void create_and_train_mla(const char* data_mla, Vector* input_pixels[TRAINING_SAMPLES], Vector* labels[TRAINING_SAMPLES]);

#endif