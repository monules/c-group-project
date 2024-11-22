#ifndef UTILS_H
#define UTILS_H

#include "constants.h"

// Function prototypes

// Convert a string to uppercase
void utils_str_to_upper(char* str);

// Convert a string to lowercase
void utils_str_to_lower(char* str);

// Trim leading and trailing whitespace from a string
void utils_trim_whitespace(char* str);

// Check if a string is a valid integer
int utils_is_valid_integer(const char* str);

// Check if a string is a valid float
int utils_is_valid_float(const char* str);

// Generate a random string of a given length
void utils_generate_random_string(char* str, int length);

// Get the current timestamp as a string
void utils_get_current_timestamp(char* timestamp);

// Utility function to get the error message for a status code
const char* utils_get_error_message(StatusCode code);

#endif // UTILS_H
