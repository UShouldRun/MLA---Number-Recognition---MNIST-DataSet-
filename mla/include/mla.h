#ifndef MLA_H
#define MLA_H

#include "nn_functions.h"
#include "dataset_handling.h"
#include <time.h>

#define MAX_ENTRY 10

void update_progress_bar(long i, double cost);
void create_and_train_mla(LogFile* logFile, const char* data_mla, Vector* input_pixels[TRAINING_SAMPLES], Vector* labels[TRAINING_SAMPLES]);

#endif
