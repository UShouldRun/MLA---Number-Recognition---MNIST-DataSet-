#include "mla/include/mla.h"

void generateFilename(char* buffer, size_t bufferSize) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, bufferSize, "tests/logfile_%Y%m%d_%H%M%S.log", t);
}

int main(int argc, char* argv[]) {
    LogFile logFile;

    generateFilename(logFile.config.logFilePath, sizeof(logFile.config.logFilePath) - 1);
    strncpy(logFile.config.logLevel, "INFO", sizeof(logFile.config.logLevel) - 1);
    logFile.config.maxFileSize = 1048576;
    logFile.currentSize = 0;
    logFile.taskCounter = 0;

    logFile.file = fopen(logFile.config.logFilePath, "a");
    if (logFile.file == NULL) {
        perror("Runtime error : main.c : 13 : Failed to open log file");
        return 1;
    }

    LogEntry logEntry;
    initializeTask(&logEntry.task, logFile.taskCounter++, "int main()");
    writeLogTaskStatus(&logFile, &logEntry, "starting process complete -> read_dataset()");

    Vector* input_pixels[TRAINING_SAMPLES];
    Vector* labels[TRAINING_SAMPLES];

    read_dataset(&logFile, "data/csv-format/mnist_train.csv", TRAINING_SAMPLES, input_pixels, labels);
    writeLogTaskStatus(&logFile, &logEntry, "read_dataset() complete -> create_and_train_mla()");
    create_and_train_mla(&logFile, "data/csv-format/mnist_trained_nn.csv", input_pixels, labels);
    writeLogTaskStatus(&logFile, &logEntry, "create_and_train_mla() complete -> freeing allocated memory");

    for (int i = 0; i < TRAINING_SAMPLES; i++) {
        free_vector(input_pixels[i]);
        free_vector(labels[i]);
    }

    fclose(logFile.file);

    return EXIT_SUCCESS;
}
