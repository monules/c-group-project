#ifndef UI_H
#define UI_H

#include "constants.h"
#include "student.h"
#include "user.h"

// Function prototypes

// Initialize the user interface
StatusCode ui_init(void);

// Clean up the user interface
StatusCode ui_cleanup(void);

// Display the main menu
StatusCode ui_display_main_menu(void);

// Display the student management menu
StatusCode ui_display_student_menu(void);

// Display the user management menu
StatusCode ui_display_user_menu(void);

// Display the report generation menu
StatusCode ui_display_report_menu(void);

// Display the search menu
StatusCode ui_display_search_menu(void);

// Display the theme selection menu
StatusCode ui_display_theme_menu(void);

// Display the help menu
StatusCode ui_display_help_menu(void);

// Handle user input for the main menu
StatusCode ui_handle_main_menu_input(int choice);

// Handle user input for the student menu
StatusCode ui_handle_student_menu_input(int choice);

// Handle user input for the user menu
StatusCode ui_handle_user_menu_input(int choice);

// Handle user input for the report menu
StatusCode ui_handle_report_menu_input(int choice);

// Handle user input for the search menu
StatusCode ui_handle_search_menu_input(int choice);

// Handle user input for the theme menu
StatusCode ui_handle_theme_menu_input(int choice);

// Handle user input for the help menu
StatusCode ui_handle_help_menu_input(int choice);

// Utility function to get the error message for a status code
const char* ui_get_error_message(StatusCode code);

#endif // UI_H
