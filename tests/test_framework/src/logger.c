#include "../include/logger.h"

void initializeTask(Task* task, unsigned short taskId, const char* taskName) {
    if (task == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 4 : in function void initializeTask() : null pointer (task) passed as input\n");
        return;
    }
    task->id = taskId;
    initializeNoError(&task->lastError);
    strncpy(task->name, taskName, sizeof(task->name) - 1);
    strncpy(task->status, "Initialized", sizeof(task->status) - 1);
}

void updateTask(Task* task, const char* status) {
    if (task == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 15 : in function void updateTask() : null pointer passed as input\n");
        return;
    }
    strncpy(task->status, status, sizeof(task->status) - 1);
}

void writeLogTaskStatus(LogFile* file, LogEntry* entry, const char* status) {
    if (file == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 23 : in function writeLogTaskStatus() : null pointer (file) passed as input\n");
        return;
    }
    if (entry == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 27 : in function void writeLogTaskStatus() : null pointer (entry) passed as input\n");
        return;
    }
    updateTask(&entry->task, status);
    setLogLevel(entry, "INFO");
    strncpy(entry->message, entry->task.status, sizeof(entry->message) - 1);
    writeLogEntry(file, entry);
}

void setLogLevel(LogEntry* entry, const char* level) {
    if (entry == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 38 : in function void setLogLevel() : null pointer (entry) passed as input\n");
        return;
    }
    strncpy(entry->logLevel, level, sizeof(entry->logLevel) - 1);
}

void initializeNoError(Error *error) {
    if (error == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 45 : in function void initializeNoError() : null pointer (error) passed as input\n");
        return;
    }
    error->errorCode = NO_ERROR;
    strncpy(error->errorMessage, "No error", sizeof(error->errorMessage) - 1);
    error->errorMessage[sizeof(error->errorMessage) - 1] = '\0';
    strncpy(error->component, "", sizeof(error->component) - 1);
    error->component[sizeof(error->component) - 1] = '\0';
    strncpy(error->function, "", sizeof(error->function) - 1);
    error->function[sizeof(error->function) - 1] = '\0';
}

void setError(Error* error, int errorCode, const char* errorMessage, const char* component, const char* function) {
    if (error == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 60 : in function void setError() : null pointer (error) passed as input\n");
        return;
    }
    error->errorCode = errorCode;
    strncpy(error->errorMessage, errorMessage, sizeof(error->errorMessage) - 1);
    strncpy(error->component, component, sizeof(error->component) - 1);
    strncpy(error->function, function, sizeof(error->function) - 1);
    error->errorMessage[sizeof(error->errorMessage) - 1] = '\0';
    error->component[sizeof(error->component) - 1] = '\0';
    error->function[sizeof(error->function) - 1] = '\0';
}

void logError(LogFile* logFile, Task* task, int errorCode, const char* errorMessage, const char* function) {
    if (logFile == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 74 : in function void logError() : null pointer (logFile) passed as input\n");
        return;
    }
    if (task == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 78 : in function void logError() : null pointer (task) passed as input\n");
        return;
    }
    setError(&task->lastError, errorCode, errorMessage, task->name, function);
    strncpy(task->status, "Failed", sizeof(task->status) - 1);
    task->status[sizeof(task->status) - 1] = '\0';
    fprintf(logFile->file, "Runtime error : %s::%s : Code -> %d : Message -> %s\n", task->name, function, errorCode, errorMessage);
    fflush(logFile->file);
}

void writeLogEntry(LogFile* logFile, LogEntry* entry) {
    if (logFile == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 90 : in function writeLogEntry : null pointer (logFile) passed as input\n");
        return;
    }
    if (entry == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 94 : in function writeLogEntry : null pointer (entry) passed as input\n");
        return;
    }
    char buffer[512];
    time(&entry->timestamp);
    struct tm* tm_info = localtime(&entry->timestamp);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(logFile->file, "%s [%s] %s\n", buffer, entry->logLevel, entry->message);
    logFile->currentSize += strlen(buffer) + 1;
    fflush(logFile->file);
}

void writeObjectData(LogFile* logFile, LogEntry* entry, void* ptr, void func_ptr(LogFile* file, void* object)) {
    if (logFile == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 108 : in function writeObjectData : null pointer (logFile) passed as input\n");
        return;
    }
    if (entry == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 112 : in function writeObjectData : null pointer (entry) passed as input\n");
        return;
    }
    if (ptr == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 116 : in function writeObjectData : null pointer (ptr) passed as input\n");
        return;
    }
    if (func_ptr == NULL) {
        fprintf(stderr, "Runtime error : tests/test_framework/src/logger.c : 120 : in function writeObjectData : null pointer (func_ptr) passed as input\n");
        return;
    }
    char buffer[1024];
    time(&entry->timestamp);
    struct tm* tm_info = localtime(&entry->timestamp);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(logFile->file, "%s [%s] %s\n", buffer, entry->logLevel, entry->message);
    func_ptr(logFile, ptr);
    logFile->currentSize += strlen(buffer) + 1;
    fflush(logFile->file);
}
