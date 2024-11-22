#include "search.h"
#include <stdio.h>
#include <string.h>

// Search for students by name
StatusCode search_by_name(const char* name, Student* results, int* count) {
    if (!name || !results || !count) {
        return ERROR_INVALID_INPUT;
    }

    Student students[MAX_STUDENTS];
    int total_students;
    StatusCode status = student_list(students, &total_students);
    if (status != SUCCESS) {
        return status;
    }

    *count = 0;
    for (int i = 0; i < total_students; i++) {
        if (strstr(students[i].name, name) != NULL) {
            results[(*count)++] = students[i];
        }
    }

    return SUCCESS;
}

// Search for students by student ID
StatusCode search_by_id(const char* id, Student* result) {
    if (!id || !result) {
        return ERROR_INVALID_INPUT;
    }

    return student_get(id, result);
}

// Search for students by department
StatusCode search_by_department(const char* department, Student* results, int* count) {
    if (!department || !results || !count) {
        return ERROR_INVALID_INPUT;
    }

    Student students[MAX_STUDENTS];
    int total_students;
    StatusCode status = student_list(students, &total_students);
    if (status != SUCCESS) {
        return status;
    }

    *count = 0;
    for (int i = 0; i < total_students; i++) {
        if (strcmp(students[i].department, department) == 0) {
            results[(*count)++] = students[i];
        }
    }

    return SUCCESS;
}

// Search for students by GPA range
StatusCode search_by_gpa_range(float min_gpa, float max_gpa, Student* results, int* count) {
    if (!results || !count) {
        return ERROR_INVALID_INPUT;
    }

    Student students[MAX_STUDENTS];
    int total_students;
    StatusCode status = student_list(students, &total_students);
    if (status != SUCCESS) {
        return status;
    }

    *count = 0;
    for (int i = 0; i < total_students; i++) {
        if (students[i].current_gpa >= min_gpa && students[i].current_gpa <= max_gpa) {
            results[(*count)++] = students[i];
        }
    }

    return SUCCESS;
}

// Search for students by attendance percentage range
StatusCode search_by_attendance_range(float min_attendance, float max_attendance, Student* results, int* count) {
    if (!results || !count) {
        return ERROR_INVALID_INPUT;
    }

    Student students[MAX_STUDENTS];
    int total_students;
    StatusCode status = student_list(students, &total_students);
    if (status != SUCCESS) {
        return status;
    }

    *count = 0;
    for (int i = 0; i < total_students; i++) {
        if (students[i].courses[0].attendance_percentage >= min_attendance &&
            students[i].courses[0].attendance_percentage <= max_attendance) {
            results[(*count)++] = students[i];
        }
    }

    return SUCCESS;
}

// Search for students by course code
StatusCode search_by_course_code(const char* course_code, Student* results, int* count) {
    if (!course_code || !results || !count) {
        return ERROR_INVALID_INPUT;
    }

    Student students[MAX_STUDENTS];
    int total_students;
    StatusCode status = student_list(students, &total_students);
    if (status != SUCCESS) {
        return status;
    }

    *count = 0;
    for (int i = 0; i < total_students; i++) {
        for (int j = 0; j < students[i].course_count; j++) {
            if (strcmp(students[i].courses[j].code, course_code) == 0) {
                results[(*count)++] = students[i];
                break;
            }
        }
    }

    return SUCCESS;
}

// Search for students by multiple criteria
StatusCode search_by_multiple_criteria(const char* name, const char* id, const char* department, float min_gpa, float max_gpa, float min_attendance, float max_attendance, Student* results, int* count) {
    if (!results || !count) {
        return ERROR_INVALID_INPUT;
    }

    Student students[MAX_STUDENTS];
    int total_students;
    StatusCode status = student_list(students, &total_students);
    if (status != SUCCESS) {
        return status;
    }

    *count = 0;
    for (int i = 0; i < total_students; i++) {
        int match = 1;

        if (name && strstr(students[i].name, name) == NULL) {
            match = 0;
        }

        if (id && strcmp(students[i].id, id) != 0) {
            match = 0;
        }

        if (department && strcmp(students[i].department, department) != 0) {
            match = 0;
        }

        if (students[i].current_gpa < min_gpa || students[i].current_gpa > max_gpa) {
            match = 0;
        }

        if (students[i].courses[0].attendance_percentage < min_attendance ||
            students[i].courses[0].attendance_percentage > max_attendance) {
            match = 0;
        }

        if (match) {
            results[(*count)++] = students[i];
        }
    }

    return SUCCESS;
}

// Get error message for status code
const char* search_get_error_message(StatusCode code) {
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
