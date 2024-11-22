#ifndef GPA_CALCULATOR_H
#define GPA_CALCULATOR_H

#include "constants.h"
#include "student.h"

// Function prototypes

// Calculate the GPA for a specific student
StatusCode gpa_calculate_student_gpa(const char* student_id, float* gpa);

// Calculate the GPA for all students
StatusCode gpa_calculate_all_students_gpa(float* gpa_array, int* count);

// Calculate the weighted GPA based on course credit hours
StatusCode gpa_calculate_weighted_gpa(const Course* courses, int course_count, float* gpa);

// Utility function to convert grade to GPA points
float gpa_grade_to_points(GradeScale grade);

// Utility function to get the error message for a status code
const char* gpa_get_error_message(StatusCode code);

#endif // GPA_CALCULATOR_H
