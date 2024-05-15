#include "mla/include/mla.h"

int main() {

    Vector* input_pixels[TRAINING_SAMPLES];
    Vector* labels[TRAINING_SAMPLES];

    read_dataset("data/csv-format/mnist_train.csv", TRAINING_SAMPLES, input_pixels, labels);
    create_and_train_mla("data/csv-format/mnist_trained_nn.csv", input_pixels, labels);

    for (int i = 0; i < TRAINING_SAMPLES; i++) {
        free_vector(input_pixels[i]);
        free_vector(labels[i]);
    }

    return EXIT_SUCCESS;
}
