#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "constants.h"
#include "student.h"

// Function prototypes

// Generate a bar chart for GPA distribution
StatusCode visualization_generate_gpa_bar_chart(const Student* students, int count);

// Generate a pie chart for attendance distribution
StatusCode visualization_generate_attendance_pie_chart(const Student* students, int count);

// Generate a bar chart for course enrollment
StatusCode visualization_generate_course_enrollment_bar_chart(const Student* students, int count);

// Utility function to get the error message for a status code
const char* visualization_get_error_message(StatusCode code);

#endif // VISUALIZATION_H
