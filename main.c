#include "ui.h"
#include "student.h"
#include "user.h"  
#include "file_handler.h"
#include "encryption.h"
#include "utils.h"
#include "gpa_calculator.h"
#include "reports.h"
#include "search.h"
#include "visualization.h"
#include "backup.h"
#include "sort.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // Initialize the system
    StatusCode status;

    status = student_init();
    if (status != SUCCESS) {
        printf("Failed to initialize student management system: %s\n", student_get_error_message(status));
        return 1;
    }

    status = user_init();
    if (status != SUCCESS) {
        printf("Failed to initialize user management system: %s\n", user_get_error_message(status));
        return 1;
    }

    status = ui_init();
    if (status != SUCCESS) {
        printf("Failed to initialize user interface: %s\n", ui_get_error_message(status));
        return 1;
    }

    // Main loop
    int choice;
    while (1) {
        ui_display_main_menu();
        scanf("%d", &choice);
        ui_handle_main_menu_input(choice);
    }

    // Clean up the system
    status = student_cleanup();
    if (status != SUCCESS) {
        printf("Failed to clean up student management system: %s\n", student_get_error_message(status));
    }

    status = user_cleanup();
    if (status != SUCCESS) {
        printf("Failed to clean up user management system: %s\n", user_get_error_message(status));
    }

    status = ui_cleanup();
    if (status != SUCCESS) {
        printf("Failed to clean up user interface: %s\n", ui_get_error_message(status));
    }

    return 0;
}
