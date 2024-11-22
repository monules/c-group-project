#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include "constants.h"

// Function prototypes

// Encrypt a string using a custom encryption method
StatusCode encrypt_string(const char* input, char* output, int length);

// Decrypt a string using the custom encryption method
StatusCode decrypt_string(const char* input, char* output, int length);

// Utility function to get the error message for a status code
const char* encryption_get_error_message(StatusCode code);

#endif // ENCRYPTION_H
