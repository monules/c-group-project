#include "visualization.h"
#include <stdio.h>
#include <string.h>

// Generate a bar chart for GPA distribution
StatusCode visualization_generate_gpa_bar_chart(const Student* students, int count) {
    if (!students || count <= 0) {
        return ERROR_INVALID_INPUT;
    }

    int gpa_counts[5] = {0}; // 0-1, 1-2, 2-3, 3-4, 4-5

    for (int i = 0; i < count; i++) {
        float gpa = students[i].current_gpa;
        if (gpa >= 0.0 && gpa < 1.0) gpa_counts[0]++;
        else if (gpa >= 1.0 && gpa < 2.0) gpa_counts[1]++;
        else if (gpa >= 2.0 && gpa < 3.0) gpa_counts[2]++;
        else if (gpa >= 3.0 && gpa < 4.0) gpa_counts[3]++;
        else if (gpa >= 4.0 && gpa <= 5.0) gpa_counts[4]++;
    }

    printf("GPA Distribution Bar Chart:\n");
    printf("===========================\n");
    for (int i = 0; i < 5; i++) {
        printf("%d-%d: ", i, i + 1);
        for (int j = 0; j < gpa_counts[i]; j++) {
            printf("*");
        }
        printf("\n");
    }

    return SUCCESS;
}

// Generate a pie chart for attendance distribution
StatusCode visualization_generate_attendance_pie_chart(const Student* students, int count) {
    if (!students || count <= 0) {
        return ERROR_INVALID_INPUT;
    }

    int attendance_counts[4] = {0}; // 0-25, 25-50, 50-75, 75-100

    for (int i = 0; i < count; i++) {
        float attendance = students[i].courses[0].attendance_percentage;
        if (attendance >= 0.0 && attendance < 25.0) attendance_counts[0]++;
        else if (attendance >= 25.0 && attendance < 50.0) attendance_counts[1]++;
        else if (attendance >= 50.0 && attendance < 75.0) attendance_counts[2]++;
        else if (attendance >= 75.0 && attendance <= 100.0) attendance_counts[3]++;
    }

    printf("Attendance Distribution Pie Chart:\n");
    printf("=================================\n");
    for (int i = 0; i < 4; i++) {
        printf("%d-%d%%: %d\n", i * 25, (i + 1) * 25, attendance_counts[i]);
    }

    return SUCCESS;
}

// Generate a bar chart for course enrollment
StatusCode visualization_generate_course_enrollment_bar_chart(const Student* students, int count) {
    if (!students || count <= 0) {
        return ERROR_INVALID_INPUT;
    }

    int course_counts[MAX_COURSES] = {0};
    char course_codes[MAX_COURSES][MAX_STRING];
    int unique_courses = 0;

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < students[i].course_count; j++) {
            int found = 0;
            for (int k = 0; k < unique_courses; k++) {
                if (strcmp(students[i].courses[j].code, course_codes[k]) == 0) {
                    course_counts[k]++;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                strcpy(course_codes[unique_courses], students[i].courses[j].code);
                course_counts[unique_courses]++;
                unique_courses++;
            }
        }
    }

    printf("Course Enrollment Bar Chart:\n");
    printf("============================\n");
    for (int i = 0; i < unique_courses; i++) {
        printf("%s: ", course_codes[i]);
        for (int j = 0; j < course_counts[i]; j++) {
            printf("*");
        }
        printf("\n");
    }

    return SUCCESS;
}

// Get error message for status code
const char* visualization_get_error_message(StatusCode code) {
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
