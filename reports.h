#ifndef REPORTS_H
#define REPORTS_H

#include "constants.h"
#include "student.h"
#include "user.h"

// Function prototypes

// Generate a GPA summary report for all students
StatusCode reports_generate_gpa_summary(const Student* students, int count, ExportFormat format);

// Generate an attendance report for all students
StatusCode reports_generate_attendance_summary(const Student* students, int count, ExportFormat format);

// Generate a detailed report for a specific student
StatusCode reports_generate_student_report(const Student* student, ExportFormat format);

// Generate a user activity log report
StatusCode reports_generate_user_activity_log(const User* users, int count, ExportFormat format);

// Generate a course enrollment report
StatusCode reports_generate_course_enrollment(const Student* students, int count, ExportFormat format);

// Utility function to get the error message for a status code
const char* reports_get_error_message(StatusCode code);

#endif // REPORTS_H
