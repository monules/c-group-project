#ifndef SORT_H
#define SORT_H

#include "constants.h"
#include "student.h"

// Function prototypes

// Sort students by multiple criteria
StatusCode sort_students_by_criteria(Student* students, int count, int (*compare)(const void*, const void*));

// Comparison function for sorting by name
int compare_students_by_name(const void* a, const void* b);

// Comparison function for sorting by GPA
int compare_students_by_gpa(const void* a, const void* b);

// Comparison function for sorting by attendance
int compare_students_by_attendance(const void* a, const void* b);

// Comparison function for sorting by name and GPA
int compare_students_by_name_and_gpa(const void* a, const void* b);

// Comparison function for sorting by GPA and attendance
int compare_students_by_gpa_and_attendance(const void* a, const void* b);

// Utility function to get the error message for a status code
const char* sort_get_error_message(StatusCode code);

#endif // SORT_H
