#include "encryption.h"
#include <stdio.h>
#include <string.h>

// Custom encryption key (for demonstration purposes)
#define ENCRYPTION_KEY 0xAA

// Encrypt a string using a custom encryption method
StatusCode encrypt_string(const char* input, char* output, int length) {
    if (!input || !output || length <= 0) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < length; i++) {
        output[i] = input[i] ^ ENCRYPTION_KEY;
    }
    output[length] = '\0';

    return SUCCESS;
}

// Decrypt a string using the custom encryption method
StatusCode decrypt_string(const char* input, char* output, int length) {
    if (!input || !output || length <= 0) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < length; i++) {
        output[i] = input[i] ^ ENCRYPTION_KEY;
    }
    output[length] = '\0';

    return SUCCESS;
}

// Get error message for status code
const char* encryption_get_error_message(StatusCode code) {
    static const char* messages[] = {
        [SUCCESS] = "Operation completed successfully",
        [ERROR_FILE_NOT_FOUND] = "File not found",
        [ERROR_PERMISSION_DENIED] = "Permission denied",
        [ERROR_INVALID_INPUT] = "Invalid input provided",
        [ERROR_MEMORY_ALLOCATION] = "Memory allocation failed",
        [ERROR_DATABASE_FULL] = "Database is full",
        [ERROR_STUDENT_NOT_FOUND] = "Student not found",
        [ERROR_ENCRYPTION_FAILED] = "Encryption failed",
        [ERROR_INVALID_GRADE] = "Invalid grade provided"
    };

    if (code >= 0 && code < sizeof(messages)/sizeof(messages[0])) {
        return messages[code];
    }
    return "Unknown error";
}
