#ifndef SEARCH_H
#define SEARCH_H

#include "constants.h"
#include "student.h"

// Function prototypes

// Search for students by name
StatusCode search_by_name(const char* name, Student* results, int* count);

// Search for students by student ID
StatusCode search_by_id(const char* id, Student* result);

// Search for students by department
StatusCode search_by_department(const char* department, Student* results, int* count);

// Search for students by GPA range
StatusCode search_by_gpa_range(float min_gpa, float max_gpa, Student* results, int* count);

// Search for students by attendance percentage range
StatusCode search_by_attendance_range(float min_attendance, float max_attendance, Student* results, int* count);

// Search for students by course code
StatusCode search_by_course_code(const char* course_code, Student* results, int* count);

// Search for students by multiple criteria
StatusCode search_by_multiple_criteria(const char* name, const char* id, const char* department, float min_gpa, float max_gpa, float min_attendance, float max_attendance, Student* results, int* count);

// Utility function to get the error message for a status code
const char* search_get_error_message(StatusCode code);

#endif // SEARCH_H
