#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Convert a string to uppercase
void utils_str_to_upper(char* str) {
    if (!str) return;
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

// Convert a string to lowercase
void utils_str_to_lower(char* str) {
    if (!str) return;
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Trim leading and trailing whitespace from a string
void utils_trim_whitespace(char* str) {
    if (!str) return;

    // Trim leading whitespace
    char* start = str;
    while (isspace(*start)) start++;

    // Trim trailing whitespace
    char* end = str + strlen(str) - 1;
    while (end > start && isspace(*end)) end--;

    // Null-terminate the trimmed string
    *(end + 1) = '\0';

    // Shift the trimmed string to the beginning of the original string
    if (start != str) {
        memmove(str, start, end - start + 2);
    }
}

// Check if a string is a valid integer
int utils_is_valid_integer(const char* str) {
    if (!str || *str == '\0') return 0;

    char* endptr;
    strtol(str, &endptr, 10);

    return *endptr == '\0';
}

// Check if a string is a valid float
int utils_is_valid_float(const char* str) {
    if (!str || *str == '\0') return 0;

    char* endptr;
    strtof(str, &endptr);

    return *endptr == '\0';
}

// Generate a random string of a given length
void utils_generate_random_string(char* str, int length) {
    if (!str || length <= 0) return;

    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int charset_size = sizeof(charset) - 1;

    for (int i = 0; i < length; i++) {
        str[i] = charset[rand() % charset_size];
    }
    str[length] = '\0';
}

// Get the current timestamp as a string
void utils_get_current_timestamp(char* timestamp) {
    if (!timestamp) return;

    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(timestamp, MAX_STRING, "%Y-%m-%d %H:%M:%S", tm_info);
}

// Get error message for status code
const char* utils_get_error_message(StatusCode code) {
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
