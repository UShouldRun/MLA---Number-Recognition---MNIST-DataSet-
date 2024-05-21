#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int errorCode;
    char errorMessage[256];
    char component[50];
    char function[50];
} Error;

typedef struct {
    unsigned short id; 
    char name[50];
    char status[256];
    Error lastError;
} Task;

typedef struct {
    Task programStatus;
    Task loggerStatus;
} SystemStatus;

typedef struct {
    time_t timestamp;
    char logLevel[10];
    char message[256];
    Task task;
} LogEntry;

typedef struct {
    char logFilePath[256];  // Path to the log file
    char logLevel[10];      // Minimum log level to record
    int maxFileSize;        // Maximum size of the log file
} LogConfig;

typedef struct {
    FILE *file;
    LogConfig config;
    size_t currentSize;
    size_t taskCounter;
} LogFile;

typedef enum {
    NO_ERROR = 0,

    // Memory allocation errors
    ERR_MEMORY_ALLOCATION_FAILURE = -12,  // ENOMEM

    // File I/O errors
    ERR_FILE_IO_ERROR = -5,               // EIO
    ERR_NO_SUCH_FILE = -2,                // ENOENT
    ERR_PERMISSION_DENIED = -13,          // EACCES
    ERR_FILE_EXISTS = -17,                // EEXIST

    // Invalid input parameters
    ERR_INVALID_ARGUMENT = -22,           // EINVAL
    ERR_RESULT_OUT_OF_RANGE = -34,        // ERANGE

    // System call errors
    ERR_INTERRUPTED_SYSTEM_CALL = -4,     // EINTR
    ERR_BROKEN_PIPE = -32,                // EPIPE
    ERR_CONNECTION_TIMED_OUT = -110,      // ETIMEDOUT

    // Network-related errors
    ERR_CONNECTION_REFUSED = -111,        // ECONNREFUSED
    ERR_NO_ROUTE_TO_HOST = -113,          // EHOSTUNREACH
    ERR_ADDRESS_ALREADY_IN_USE = -98,     // EADDRINUSE

    // Thread-related errors
    ERR_RESOURCE_DEADLOCK_WOULD_OCCUR = -11,  // EDEADLK
    ERR_INSUFFICIENT_MEMORY = -12             // ENOMEM
} ErrorCode;

void initializeTask(Task* task, unsigned short taskId, const char* taskName);
void updateTask(Task* task, const char* status);
void writeLogTaskStatus(LogFile* file, LogEntry* entry, const char* status);

void setLogLevel(LogEntry* entry, const char* level);

void initializeNoError(Error *error);
void setError(Error* error, int errorCode, const char* errorMessage, const char* component, const char* function);
void logError(LogFile* logFile, Task* task, int errorCode, const char* errorMessage, const char* function);

void writeLogEntry(LogFile* logFile, LogEntry* entry);
void writeObjectData(LogFile* logFile, LogEntry* entry, void* ptr, void func_ptr(LogFile* file, void* object));

#endif
