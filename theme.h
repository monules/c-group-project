#ifndef THEME_H
#define THEME_H

#include "constants.h"

// Function prototypes

// Set the theme for the user interface
StatusCode theme_set(ThemeType theme);

// Get the current theme
StatusCode theme_get(ThemeType* theme);

// Toggle between light and dark themes
StatusCode theme_toggle(void);

// Utility function to get the error message for a status code
const char* theme_get_error_message(StatusCode code);

#endif // THEME_H
