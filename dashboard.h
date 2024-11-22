#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "constants.h"
#include "student.h"
#include "user.h"

// Function prototypes

// Display the admin dashboard
StatusCode dashboard_display(void);

// Display key data points on the dashboard
StatusCode dashboard_display_key_data(void);

// Display recent changes in student records
StatusCode dashboard_display_recent_changes(void);

// Display attendance trends
StatusCode dashboard_display_attendance_trends(void);

// Display average GPA
StatusCode dashboard_display_average_gpa(void);

// Utility function to get the error message for a status code
const char* dashboard_get_error_message(StatusCode code);

#endif // DASHBOARD_H
