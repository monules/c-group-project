#include "sort.h"
#include <stdio.h>
#include <string.h>

// Comparison function for sorting by name
int compare_students_by_name(const void* a, const void* b) {
    Student* student_a = (Student*)a;
    Student* student_b = (Student*)b;
    return strcmp(student_a->name, student_b->name);
}

// Comparison function for sorting by GPA
int compare_students_by_gpa(const void* a, const void* b) {
    Student* student_a = (Student*)a;
    Student* student_b = (Student*)b;
    if (student_a->current_gpa < student_b->current_gpa) return -1;
    if (student_a->current_gpa > student_b->current_gpa) return 1;
    return 0;
}

// Comparison function for sorting by attendance
int compare_students_by_attendance(const void* a, const void* b) {
    Student* student_a = (Student*)a;
    Student* student_b = (Student*)b;
    if (student_a->courses[0].attendance_percentage < student_b->courses[0].attendance_percentage) return -1;
    if (student_a->courses[0].attendance_percentage > student_b->courses[0].attendance_percentage) return 1;
    return 0;
}

// Comparison function for sorting by name and GPA
int compare_students_by_name_and_gpa(const void* a, const void* b) {
    int name_cmp = compare_students_by_name(a, b);
    if (name_cmp != 0) return name_cmp;
    return compare_students_by_gpa(a, b);
}

// Comparison function for sorting by GPA and attendance
int compare_students_by_gpa_and_attendance(const void* a, const void* b) {
    int gpa_cmp = compare_students_by_gpa(a, b);
    if (gpa_cmp != 0) return gpa_cmp;
    return compare_students_by_attendance(a, b);
}

// Sort students by multiple criteria
StatusCode sort_students_by_criteria(Student* students, int count, int (*compare)(const void*, const void*)) {
    if (!students || count <= 0 || !compare) {
        return ERROR_INVALID_INPUT;
    }

    qsort(students, count, sizeof(Student), compare);
    return SUCCESS;
}

// Get error message for status code
const char* sort_get_error_message(StatusCode code) {
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
