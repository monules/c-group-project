#include "dashboard.h"
#include "student.h"
#include "user.h"
#include <stdio.h>

// Display the admin dashboard
StatusCode dashboard_display(void) {
    printf("========================================\n");
    printf("            Admin Dashboard             \n");
    printf("========================================\n\n");

    StatusCode status;

    status = dashboard_display_key_data();
    if (status != SUCCESS) {
        return status;
    }

    status = dashboard_display_recent_changes();
    if (status != SUCCESS) {
        return status;
    }

    status = dashboard_display_attendance_trends();
    if (status != SUCCESS) {
        return status;
    }

    status = dashboard_display_average_gpa();
    if (status != SUCCESS) {
        return status;
    }

    return SUCCESS;
}

// Display key data points on the dashboard
StatusCode dashboard_display_key_data(void) {
    Student students[MAX_STUDENTS];
    int total_students;
    StatusCode status = student_list(students, &total_students);
    if (status != SUCCESS) {
        return status;
    }

    User users[MAX_USERS];
    int total_users;
    status = user_list(users, &total_users);
    if (status != SUCCESS) {
        return status;
    }

    printf("Key Data Points:\n");
    printf("----------------\n");
    printf("Total Students: %d\n", total_students);
    printf("Total Users: %d\n", total_users);
    printf("\n");

    return SUCCESS;
}

// Display recent changes in student records
StatusCode dashboard_display_recent_changes(void) {
    Student students[MAX_STUDENTS];
    int total_students;
    StatusCode status = student_list(students, &total_students);
    if (status != SUCCESS) {
        return status;
    }

    printf("Recent Changes in Student Records:\n");
    printf("----------------------------------\n");

    for (int i = 0; i < total_students; i++) {
        if (students[i].last_modified != 0) {
            printf("Student ID: %s\n", students[i].id);
            printf("Name: %s\n", students[i].name);
            printf("Last Modified: %s", ctime(&students[i].last_modified));
            printf("\n");
        }
    }

    return SUCCESS;
}

// Display attendance trends
StatusCode dashboard_display_attendance_trends(void) {
    Student students[MAX_STUDENTS];
    int total_students;
    StatusCode status = student_list(students, &total_students);
    if (status != SUCCESS) {
        return status;
    }

    printf("Attendance Trends:\n");
    printf("------------------\n");

    for (int i = 0; i < total_students; i++) {
        printf("Student ID: %s\n", students[i].id);
        printf("Name: %s\n", students[i].name);
        printf("Attendance: %.2f%%\n", students[i].courses[0].attendance_percentage);
        printf("\n");
    }

    return SUCCESS;
}

// Display average GPA
StatusCode dashboard_display_average_gpa(void) {
    Student students[MAX_STUDENTS];
    int total_students;
    StatusCode status = student_list(students, &total_students);
    if (status != SUCCESS) {
        return status;
    }

    float total_gpa = 0.0f;
    for (int i = 0; i < total_students; i++) {
        total_gpa += students[i].current_gpa;
    }

    float average_gpa = total_students > 0 ? total_gpa / total_students : 0.0f;

    printf("Average GPA:\n");
    printf("------------\n");
    printf("Average GPA: %.2f\n", average_gpa);
    printf("\n");

    return SUCCESS;
}

// Get error message for status code
const char* dashboard_get_error_message(StatusCode code) {
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
