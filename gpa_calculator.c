#include "gpa_calculator.h"
#include <stdio.h>
#include <string.h>

// GPA conversion table
static const float grade_points[] = {
    [GRADE_A_PLUS]  = 4.0f,
    [GRADE_A]       = 4.0f,
    [GRADE_A_MINUS] = 3.7f,
    [GRADE_B_PLUS]  = 3.3f,
    [GRADE_B]       = 3.0f,
    [GRADE_B_MINUS] = 2.7f,
    [GRADE_C_PLUS]  = 2.3f,
    [GRADE_C]       = 2.0f,
    [GRADE_C_MINUS] = 1.7f,
    [GRADE_D_PLUS]  = 1.3f,
    [GRADE_D]       = 1.0f,
    [GRADE_F]       = 0.0f
};

// Calculate the GPA for a specific student
StatusCode gpa_calculate_student_gpa(const char* student_id, float* gpa) {
    Student student;
    StatusCode status = student_get(student_id, &student);
    if (status != SUCCESS) {
        return status;
    }

    return gpa_calculate_weighted_gpa(student.courses, student.course_count, gpa);
}

// Calculate the GPA for all students
StatusCode gpa_calculate_all_students_gpa(float* gpa_array, int* count) {
    Student students[MAX_STUDENTS];
    int total_students;
    StatusCode status = student_list(students, &total_students);
    if (status != SUCCESS) {
        return status;
    }

    *count = total_students;
    for (int i = 0; i < total_students; i++) {
        status = gpa_calculate_weighted_gpa(students[i].courses, students[i].course_count, &gpa_array[i]);
        if (status != SUCCESS) {
            return status;
        }
    }

    return SUCCESS;
}

// Calculate the weighted GPA based on course credit hours
StatusCode gpa_calculate_weighted_gpa(const Course* courses, int course_count, float* gpa) {
    if (course_count <= 0) {
        *gpa = 0.0f;
        return SUCCESS;
    }

    float total_points = 0.0f;
    int total_credits = 0;

    for (int i = 0; i < course_count; i++) {
        float grade_point = gpa_grade_to_points(courses[i].grade);
        total_points += grade_point * courses[i].credits;
        total_credits += courses[i].credits;
    }

    if (total_credits > 0) {
        *gpa = total_points / total_credits;
    } else {
        *gpa = 0.0f;
    }

    return SUCCESS;
}

// Convert grade to GPA points
float gpa_grade_to_points(GradeScale grade) {
    if (grade >= GRADE_A_PLUS && grade <= GRADE_F) {
        return grade_points[grade];
    }
    return 0.0f;
}

// Get error message for status code
const char* gpa_get_error_message(StatusCode code) {
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
