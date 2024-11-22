#include "ui.h"
#include "student.h"
#include "user.h"
#include "reports.h"
#include "search.h"
#include "theme.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variable to store the current theme
static ThemeType current_theme = THEME_LIGHT;

// Function to print colored text based on the current theme
void print_colored(const char* text, const char* color) {
    if (current_theme == THEME_LIGHT) {
        printf("%s", text);
    } else {
        if (strcmp(color, "red") == 0) {
            printf("\033[0;31m%s\033[0m", text);
        } else if (strcmp(color, "green") == 0) {
            printf("\033[0;32m%s\033[0m", text);
        } else if (strcmp(color, "blue") == 0) {
            printf("\033[0;34m%s\033[0m", text);
        } else {
            printf("%s", text);
        }
    }
}

// Initialize the user interface
StatusCode ui_init(void) {
    // Initialize any necessary UI components here
    return SUCCESS;
}

// Clean up the user interface
StatusCode ui_cleanup(void) {
    // Clean up any allocated resources here
    return SUCCESS;
}

// Display the main menu
StatusCode ui_display_main_menu(void) {
    print_colored("========================================\n", "green");
    print_colored("            Main Menu                   \n", "green");
    print_colored("========================================\n", "green");
    printf("1. Student Management\n");
    printf("2. User Management\n");
    printf("3. Report Generation\n");
    printf("4. Search\n");
    printf("5. Theme Selection\n");
    printf("6. Help\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
    return SUCCESS;
}

// Display the student management menu
StatusCode ui_display_student_menu(void) {
    print_colored("========================================\n", "blue");
    print_colored("         Student Management Menu        \n", "blue");
    print_colored("========================================\n", "blue");
    printf("1. Add Student\n");
    printf("2. Update Student\n");
    printf("3. Delete Student\n");
    printf("4. View Student\n");
    printf("5. Back to Main Menu\n");
    printf("Enter your choice: ");
    return SUCCESS;
}

// Display the user management menu
StatusCode ui_display_user_menu(void) {
    print_colored("========================================\n", "blue");
    print_colored("          User Management Menu          \n", "blue");
    print_colored("========================================\n", "blue");
    printf("1. Add User\n");
    printf("2. Update User\n");
    printf("3. Delete User\n");
    printf("4. View User\n");
    printf("5. Back to Main Menu\n");
    printf("Enter your choice: ");
    return SUCCESS;
}

// Display the report generation menu
StatusCode ui_display_report_menu(void) {
    print_colored("========================================\n", "blue");
    print_colored("         Report Generation Menu         \n", "blue");
    print_colored("========================================\n", "blue");
    printf("1. Generate GPA Summary\n");
    printf("2. Generate Attendance Summary\n");
    printf("3. Generate Student Report\n");
    printf("4. Generate User Activity Log\n");
    printf("5. Generate Course Enrollment Report\n");
    printf("6. Back to Main Menu\n");
    printf("Enter your choice: ");
    return SUCCESS;
}

// Display the search menu
StatusCode ui_display_search_menu(void) {
    print_colored("========================================\n", "blue");
    print_colored("              Search Menu               \n", "blue");
    print_colored("========================================\n", "blue");
    printf("1. Search by Name\n");
    printf("2. Search by ID\n");
    printf("3. Search by Department\n");
    printf("4. Search by GPA Range\n");
    printf("5. Search by Attendance Range\n");
    printf("6. Search by Course Code\n");
    printf("7. Search by Multiple Criteria\n");
    printf("8. Back to Main Menu\n");
    printf("Enter your choice: ");
    return SUCCESS;
}

// Display the theme selection menu
StatusCode ui_display_theme_menu(void) {
    print_colored("========================================\n", "blue");
    print_colored("           Theme Selection Menu         \n", "blue");
    print_colored("========================================\n", "blue");
    printf("1. Set Light Theme\n");
    printf("2. Set Dark Theme\n");
    printf("3. Toggle Theme\n");
    printf("4. Back to Main Menu\n");
    printf("Enter your choice: ");
    return SUCCESS;
}

// Display the help menu
StatusCode ui_display_help_menu(void) {
    print_colored("========================================\n", "blue");
    print_colored("               Help Menu                \n", "blue");
    print_colored("========================================\n", "blue");
    printf("1. View General Help\n");
    printf("2. View Student Management Help\n");
    printf("3. View User Management Help\n");
    printf("4. View Report Generation Help\n");
    printf("5. View Search Help\n");
    printf("6. View Theme Selection Help\n");
    printf("7. Back to Main Menu\n");
    printf("Enter your choice: ");
    return SUCCESS;
}

// Handle user input for the main menu
StatusCode ui_handle_main_menu_input(int choice) {
    switch (choice) {
        case 1:
            ui_display_student_menu();
            break;
        case 2:
            ui_display_user_menu();
            break;
        case 3:
            ui_display_report_menu();
            break;
        case 4:
            ui_display_search_menu();
            break;
        case 5:
            ui_display_theme_menu();
            break;
        case 6:
            ui_display_help_menu();
            break;
        case 7:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
    return SUCCESS;
}

// Handle user input for the student menu
StatusCode ui_handle_student_menu_input(int choice) {
    switch (choice) {
        case 1: {
            Student student;
            student_init_structure(&student);
            printf("Enter Student ID: ");
            scanf("%s", student.id);
            printf("Enter Student Name: ");
            scanf("%s", student.name);
            printf("Enter Department: ");
            scanf("%s", student.department);
            StatusCode status = student_add(&student);
            if (status == SUCCESS) {
                printf("Student added successfully.\n");
            } else {
                printf("Failed to add student.\n");
            }
            break;
        }
        case 2: {
            char id[MAX_STRING];
            printf("Enter Student ID to update: ");
            scanf("%s", id);
            Student student;
            StatusCode status = student_get(id, &student);
            if (status == SUCCESS) {
                printf("Enter new Student Name: ");
                scanf("%s", student.name);
                printf("Enter new Department: ");
                scanf("%s", student.department);
                status = student_update(id, &student);
                if (status == SUCCESS) {
                    printf("Student updated successfully.\n");
                } else {
                    printf("Failed to update student.\n");
                }
            } else {
                printf("Student not found.\n");
            }
            break;
        }
        case 3: {
            char id[MAX_STRING];
            printf("Enter Student ID to delete: ");
            scanf("%s", id);
            StatusCode status = student_delete(id);
            if (status == SUCCESS) {
                printf("Student deleted successfully.\n");
            } else {
                printf("Failed to delete student.\n");
            }
            break;
        }
        case 4: {
            char id[MAX_STRING];
            printf("Enter Student ID to view: ");
            scanf("%s", id);
            Student student;
            StatusCode status = student_get(id, &student);
            if (status == SUCCESS) {
                printf("Student ID: %s\n", student.id);
                printf("Student Name: %s\n", student.name);
                printf("Department: %s\n", student.department);
            } else {
                printf("Student not found.\n");
            }
            break;
        }
        case 5:
            ui_display_main_menu();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
    return SUCCESS;
}

// Handle user input for the user menu
StatusCode ui_handle_user_menu_input(int choice) {
    switch (choice) {
        case 1: {
            User user;
            user_init_structure(&user);
            printf("Enter Username: ");
            scanf("%s", user.username);
            printf("Enter Password: ");
            scanf("%s", user.password);
            printf("Enter Full Name: ");
            scanf("%s", user.full_name);
            printf("Enter Email: ");
            scanf("%s", user.email);
            printf("Enter Role (0: Admin, 1: Professor, 2: Student): ");
            int role;
            scanf("%d", &role);
            user.role = (UserRole)role;
            StatusCode status = user_create(&user);
            if (status == SUCCESS) {
                printf("User added successfully.\n");
            } else {
                printf("Failed to add user.\n");
            }
            break;
        }
        case 2: {
            char username[MAX_STRING];
            printf("Enter Username to update: ");
            scanf("%s", username);
            User user;
            StatusCode status = user_get(username, &user);
            if (status == SUCCESS) {
                printf("Enter new Full Name: ");
                scanf("%s", user.full_name);
                printf("Enter new Email: ");
                scanf("%s", user.email);
                status = user_update(username, &user);
                if (status == SUCCESS) {
                    printf("User updated successfully.\n");
                } else {
                    printf("Failed to update user.\n");
                }
            } else {
                printf("User not found.\n");
            }
            break;
        }
        case 3: {
            char username[MAX_STRING];
            printf("Enter Username to delete: ");
            scanf("%s", username);
            StatusCode status = user_delete(username);
            if (status == SUCCESS) {
                printf("User deleted successfully.\n");
            } else {
                printf("Failed to delete user.\n");
            }
            break;
        }
        case 4: {
            char username[MAX_STRING];
            printf("Enter Username to view: ");
            scanf("%s", username);
            User user;
            StatusCode status = user_get(username, &user);
            if (status == SUCCESS) {
                printf("Username: %s\n", user.username);
                printf("Full Name: %s\n", user.full_name);
                printf("Email: %s\n", user.email);
                printf("Role: %s\n", user_role_to_string(user.role));
            } else {
                printf("User not found.\n");
            }
            break;
        }
        case 5:
            ui_display_main_menu();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
    return SUCCESS;
}

// Handle user input for the report menu
StatusCode ui_handle_report_menu_input(int choice) {
    switch (choice) {
        case 1: {
            Student students[MAX_STUDENTS];
            int count;
            StatusCode status = student_list(students, &count);
            if (status == SUCCESS) {
                status = reports_generate_gpa_summary(students, count, FORMAT_TEXT);
                if (status == SUCCESS) {
                    printf("GPA Summary generated successfully.\n");
                } else {
                    printf("Failed to generate GPA Summary.\n");
                }
            } else {
                printf("Failed to retrieve student list.\n");
            }
            break;
        }
        case 2: {
            Student students[MAX_STUDENTS];
            int count;
            StatusCode status = student_list(students, &count);
            if (status == SUCCESS) {
                status = reports_generate_attendance_summary(students, count, FORMAT_TEXT);
                if (status == SUCCESS) {
                    printf("Attendance Summary generated successfully.\n");
                } else {
                    printf("Failed to generate Attendance Summary.\n");
                }
            } else {
                printf("Failed to retrieve student list.\n");
            }
            break;
        }
        case 3: {
            char id[MAX_STRING];
            printf("Enter Student ID to generate report: ");
            scanf("%s", id);
            Student student;
            StatusCode status = student_get(id, &student);
            if (status == SUCCESS) {
                status = reports_generate_student_report(&student, FORMAT_TEXT);
                if (status == SUCCESS) {
                    printf("Student Report generated successfully.\n");
                } else {
                    printf("Failed to generate Student Report.\n");
                }
            } else {
                printf("Student not found.\n");
            }
            break;
        }
        case 4: {
            User users[MAX_USERS];
            int count;
            StatusCode status = user_list(users, &count);
            if (status == SUCCESS) {
                status = reports_generate_user_activity_log(users, count, FORMAT_TEXT);
                if (status == SUCCESS) {
                    printf("User Activity Log generated successfully.\n");
                } else {
                    printf("Failed to generate User Activity Log.\n");
                }
            } else {
                printf("Failed to retrieve user list.\n");
            }
            break;
        }
        case 5: {
            Student students[MAX_STUDENTS];
            int count;
            StatusCode status = student_list(students, &count);
            if (status == SUCCESS) {
                status = reports_generate_course_enrollment(students, count, FORMAT_TEXT);
                if (status == SUCCESS) {
                    printf("Course Enrollment Report generated successfully.\n");
                } else {
                    printf("Failed to generate Course Enrollment Report.\n");
                }
            } else {
                printf("Failed to retrieve student list.\n");
            }
            break;
        }
        case 6:
            ui_display_main_menu();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
    return SUCCESS;
}

// Handle user input for the search menu
StatusCode ui_handle_search_menu_input(int choice) {
    switch (choice) {
        case 1: {
            char name[MAX_STRING];
            printf("Enter Name to search: ");
            scanf("%s", name);
            Student results[MAX_STUDENTS];
            int count;
            StatusCode status = search_by_name(name, results, &count);
            if (status == SUCCESS) {
                printf("Search results:\n");
                for (int i = 0; i < count; i++) {
                    printf("Student ID: %s, Name: %s, Department: %s\n", results[i].id, results[i].name, results[i].department);
                }
            } else {
                printf("Failed to search by name.\n");
            }
            break;
        }
        case 2: {
            char id[MAX_STRING];
            printf("Enter ID to search: ");
            scanf("%s", id);
            Student result;
            StatusCode status = search_by_id(id, &result);
            if (status == SUCCESS) {
                printf("Student ID: %s, Name: %s, Department: %s\n", result.id, result.name, result.department);
            } else {
                printf("Student not found.\n");
            }
            break;
        }
        case 3: {
            char department[MAX_STRING];
            printf("Enter Department to search: ");
            scanf("%s", department);
            Student results[MAX_STUDENTS];
            int count;
            StatusCode status = search_by_department(department, results, &count);
            if (status == SUCCESS) {
                printf("Search results:\n");
                for (int i = 0; i < count; i++) {
                    printf("Student ID: %s, Name: %s, Department: %s\n", results[i].id, results[i].name, results[i].department);
                }
            } else {
                printf("Failed to search by department.\n");
            }
            break;
        }
        case 4: {
            float min_gpa, max_gpa;
            printf("Enter Min GPA: ");
            scanf("%f", &min_gpa);
            printf("Enter Max GPA: ");
            scanf("%f", &max_gpa);
            Student results[MAX_STUDENTS];
            int count;
            StatusCode status = search_by_gpa_range(min_gpa, max_gpa, results, &count);
            if (status == SUCCESS) {
                printf("Search results:\n");
                for (int i = 0; i < count; i++) {
                    printf("Student ID: %s, Name: %s, GPA: %.2f\n", results[i].id, results[i].name, results[i].current_gpa);
                }
            } else {
                printf("Failed to search by GPA range.\n");
            }
            break;
        }
        case 5: {
            float min_attendance, max_attendance;
            printf("Enter Min Attendance: ");
            scanf("%f", &min_attendance);
            printf("Enter Max Attendance: ");
            scanf("%f", &max_attendance);
            Student results[MAX_STUDENTS];
            int count;
            StatusCode status = search_by_attendance_range(min_attendance, max_attendance, results, &count);
            if (status == SUCCESS) {
                printf("Search results:\n");
                for (int i = 0; i < count; i++) {
                    printf("Student ID: %s, Name: %s, Attendance: %.2f%%\n", results[i].id, results[i].name, results[i].courses[0].attendance_percentage);
                }
            } else {
                printf("Failed to search by attendance range.\n");
            }
            break;
        }
        case 6: {
            char course_code[MAX_STRING];
            printf("Enter Course Code to search: ");
            scanf("%s", course_code);
            Student results[MAX_STUDENTS];
            int count;
            StatusCode status = search_by_course_code(course_code, results, &count);
            if (status == SUCCESS) {
                printf("Search results:\n");
                for (int i = 0; i < count; i++) {
                    printf("Student ID: %s, Name: %s, Department: %s\n", results[i].id, results[i].name, results[i].department);
                }
            } else {
                printf("Failed to search by course code.\n");
            }
            break;
        }
        case 7: {
            char name[MAX_STRING], id[MAX_STRING], department[MAX_STRING];
            float min_gpa, max_gpa, min_attendance, max_attendance;
            printf("Enter Name (leave empty if not applicable): ");
            scanf("%s", name);
            printf("Enter ID (leave empty if not applicable): ");
            scanf("%s", id);
            printf("Enter Department (leave empty if not applicable): ");
            scanf("%s", department);
            printf("Enter Min GPA (leave 0 if not applicable): ");
            scanf("%f", &min_gpa);
            printf("Enter Max GPA (leave 0 if not applicable): ");
            scanf("%f", &max_gpa);
            printf("Enter Min Attendance (leave 0 if not applicable): ");
            scanf("%f", &min_attendance);
            printf("Enter Max Attendance (leave 0 if not applicable): ");
            scanf("%f", &max_attendance);
            Student results[MAX_STUDENTS];
            int count;
            StatusCode status = search_by_multiple_criteria(name, id, department, min_gpa, max_gpa, min_attendance, max_attendance, results, &count);
            if (status == SUCCESS) {
                printf("Search results:\n");
                for (int i = 0; i < count; i++) {
                    printf("Student ID: %s, Name: %s, Department: %s\n", results[i].id, results[i].name, results[i].department);
                }
            } else {
                printf("Failed to search by multiple criteria.\n");
            }
            break;
        }
        case 8:
            ui_display_main_menu();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
    return SUCCESS;
}

// Handle user input for the theme menu
StatusCode ui_handle_theme_menu_input(int choice) {
    switch (choice) {
        case 1:
            theme_set(THEME_LIGHT);
            current_theme = THEME_LIGHT;
            printf("Light theme set.\n");
            break;
        case 2:
            theme_set(THEME_DARK);
            current_theme = THEME_DARK;
            printf("Dark theme set.\n");
            break;
        case 3:
            theme_toggle();
            theme_get(&current_theme);
            printf("Theme toggled.\n");
            break;
        case 4:
            ui_display_main_menu();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
    return SUCCESS;
}

// Handle user input for the help menu
StatusCode ui_handle_help_menu_input(int choice) {
    switch (choice) {
        case 1:
            printf("General Help:\n");
            printf("This is the main menu of the Student Management System.\n");
            printf("You can navigate through different options using the numbers provided.\n");
            break;
        case 2:
            printf("Student Management Help:\n");
            printf("This menu allows you to manage student records.\n");
            printf("You can add, update, delete, and view student information.\n");
            break;
        case 3:
            printf("User Management Help:\n");
            printf("This menu allows you to manage user accounts.\n");
            printf("You can add, update, delete, and view user information.\n");
            break;
        case 4:
            printf("Report Generation Help:\n");
            printf("This menu allows you to generate various reports.\n");
            printf("You can generate reports for GPA, attendance, student details, user activity, and course enrollment.\n");
            break;
        case 5:
            printf("Search Help:\n");
            printf("This menu allows you to search for students based on various criteria.\n");
            printf("You can search by name, ID, department, GPA range, attendance range, course code, or multiple criteria.\n");
            break;
        case 6:
            printf("Theme Selection Help:\n");
            printf("This menu allows you to change the theme of the user interface.\n");
            printf("You can choose between light and dark themes.\n");
            break;
        case 7:
            ui_display_main_menu();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
    return SUCCESS;
}

// Get error message for status code
const char* ui_get_error_message(StatusCode code) {
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
