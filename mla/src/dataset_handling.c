#include "../include/dataset_handling.h"

void read_dataset(LogFile* logFile, const char* csv_filename, const int training_samples, Vector* pixels[], Vector* labels[]) {
    LogEntry* logEntry = (LogEntry*)malloc(sizeof(LogEntry));
    initializeTask(&logEntry->task, logFile->taskCounter++, "void read_dataset()");

    void writeTaskStatus(const char* status) { writeLogTaskStatus(logFile, logEntry, status); }

    FILE *fp;
    char line[MAX_LINE_LENGTH];
    char *token;
    int num_samples = 0;

    // Open the file
    fp = fopen(csv_filename, "r");
    if (fp == NULL) {
        logError(logFile, &logEntry->task, ERR_FILE_IO_ERROR, "error opening csv file", "mla/src/dataset_handling.c : in function read_dataset : 15");
        setLogLevel(logEntry, "FATAL");
        strncpy(logEntry->message, logEntry->task.lastError.errorMessage, sizeof(logEntry->message) - 1);
        writeLogEntry(logFile, logEntry);
        exit(logEntry->task.lastError.errorCode);
    }

    writeTaskStatus("opened selected file");

    // Read the file line by line
    while (fgets(line, sizeof(line), fp) != NULL && num_samples < training_samples) {
        // Tokenize the line
        token = strtok(line, ",");
        if (token == NULL) {
            logError(logFile, &logEntry->task, ERR_FILE_IO_ERROR, "error reading csv file - empty line encountered", " mla/src/dataset_handling.c : in function read_dataset : 29");
            setLogLevel(logEntry, "ERROR");
            strncpy(logEntry->message, logEntry->task.lastError.errorMessage, sizeof(logEntry->message) - 1);
            writeLogEntry(logFile, logEntry);
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

    writeTaskStatus("dataset read successfully");
    writeLogEntry(logFile, logEntry);
}

void write_data(LogFile* logFile, const char* file, const int length, Matrix* edges[], Vector* biases[]) {
    LogEntry* logEntry = (LogEntry*)malloc(sizeof(LogEntry)); 
    initializeTask(&logEntry->task, logFile->taskCounter++, "dataset_handling.c -> void write_data()");

    void writeTaskStatus(const char* status) { writeLogTaskStatus(logFile, logEntry, status); }

    FILE *fp = fopen(file, "w");
    if (fp == NULL) {
        logError(logFile, &logEntry->task, ERR_FILE_IO_ERROR, "error opening csv file", "mla/src/dataset_handling.c : in function write_data : 72");
        setLogLevel(logEntry, "FATAL");
        strncpy(logEntry->message, logEntry->task.lastError.errorMessage, sizeof(logEntry->message) - 1);
        writeLogEntry(logFile, logEntry);
        fclose(logFile->file);
        exit(logEntry->task.lastError.errorCode);
    }

    writeTaskStatus("opened selected file");

    // Write matrix data to file
    for (int i = 0; i < length; i++) {
        for (int row = 0; row < edges[i]->rows; row++) {
            for (int col = 0; col < edges[i]->cols; col++)
                fprintf(fp, "%lf,", edges[i]->data[row][col]);
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n"); // Separate matrices with a blank line
    }

    // Write vector biases to file
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < biases[i]->len; j++)
            fprintf(fp, "%lf,", biases[i]->data[j]);
        fprintf(fp, "\n");
    }

    fclose(fp);

    writeTaskStatus("data written successfully in selected file");
}

void read_data(LogFile* logFile, const char* file, const int length, Matrix* edges[], Vector* biases[]) {
    LogEntry* logEntry = (LogEntry*)malloc(sizeof(LogEntry));
    initializeTask(&logEntry->task, logFile->taskCounter++, "dataset_handling.c -> void read_data()");

    void writeTaskStatus(const char* status) { writeLogTaskStatus(logFile, logEntry, status); }

    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        logError(logFile, &logEntry->task, ERR_FILE_IO_ERROR, "error opening csv file", "mla/src/dataset_handling.c : in function read_data : 112");
        setLogLevel(logEntry, "FATAL");
        strncpy(logEntry->message, logEntry->task.lastError.errorMessage, sizeof(logEntry->message) - 1);
        writeLogEntry(logFile, logEntry);
        fclose(logFile->file);
        exit(logEntry->task.lastError.errorCode);
    }

    writeTaskStatus("opened csv file");

    // Read matrix data from file
    for (int i = 0; i < length; i++) {
        for (int row = 0; row < edges[i]->rows; row++) {
            for (int col = 0; col < edges[i]->cols; col++) {
                if (fscanf(fp, "%lf,", &edges[i]->data[row][col]) != 1) {
                    logError(logFile, &logEntry->task, ERR_FILE_IO_ERROR, "error reading matrix data from file", "mla/src/dataset_handling.c : in function read_data : 128");
                    setLogLevel(logEntry, "FATAL");
                    strncpy(logEntry->message, logEntry->task.lastError.errorMessage, sizeof(logEntry->message) - 1);
                    writeLogEntry(logFile, logEntry);
                    fclose(logFile->file);
                    exit(logEntry->task.lastError.errorCode);
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
                logError(logFile, &logEntry->task, ERR_FILE_IO_ERROR, "error reading vector data from file", "mla/src/dataset_handling.c : in function read_data : 146");
                setLogLevel(logEntry, "FATAL");
                strncpy(logEntry->message, logEntry->task.lastError.errorMessage, sizeof(logEntry->message) - 1);
                writeLogEntry(logFile, logEntry);
                fclose(logFile->file);
                exit(logEntry->task.lastError.errorCode);
            }
        }
        // Read newline character separator
        char newLine;
        fscanf(fp, "%c", &newLine);
    }

    fclose(fp);

    writeTaskStatus("data read successfully");
}
