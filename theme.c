#include "theme.h"
#include <stdio.h>

// Global variable to store the current theme
static ThemeType current_theme = THEME_LIGHT;

// Set the theme for the user interface
StatusCode theme_set(ThemeType theme) {
    if (theme != THEME_LIGHT && theme != THEME_DARK) {
        return ERROR_INVALID_INPUT;
    }

    current_theme = theme;
    return SUCCESS;
}

// Get the current theme
StatusCode theme_get(ThemeType* theme) {
    if (!theme) {
        return ERROR_INVALID_INPUT;
    }

    *theme = current_theme;
    return SUCCESS;
}

// Toggle between light and dark themes
StatusCode theme_toggle(void) {
    current_theme = (current_theme == THEME_LIGHT) ? THEME_DARK : THEME_LIGHT;
    return SUCCESS;
}

// Get error message for status code
const char* theme_get_error_message(StatusCode code) {
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
