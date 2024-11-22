#include "reports.h"
#include "file_handler.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// Generate a GPA summary report for all students
StatusCode reports_generate_gpa_summary(const Student* students, int count, ExportFormat format) {
    if (!students || count <= 0) {
        return ERROR_INVALID_INPUT;
    }

    char report_content[MAX_STRING * MAX_STUDENTS];
    memset(report_content, 0, sizeof(report_content));

    strcat(report_content, "GPA Summary Report\n");
    strcat(report_content, "===================\n\n");

    for (int i = 0; i < count; i++) {
        char student_info[MAX_STRING];
        snprintf(student_info, MAX_STRING, "Student ID: %s\nName: %s\nGPA: %.2f\n\n",
                students[i].id, students[i].name, students[i].current_gpa);
        strcat(report_content, student_info);
    }

    char report_name[MAX_STRING];
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(report_name, MAX_STRING, "gpa_summary_%Y%m%d%H%M%S", tm_info);

    return file_handler_generate_report(report_name, report_content, format);
}

// Generate an attendance report for all students
StatusCode reports_generate_attendance_summary(const Student* students, int count, ExportFormat format) {
    if (!students || count <= 0) {
        return ERROR_INVALID_INPUT;
    }

    char report_content[MAX_STRING * MAX_STUDENTS];
    memset(report_content, 0, sizeof(report_content));

    strcat(report_content, "Attendance Summary Report\n");
    strcat(report_content, "=========================\n\n");

    for (int i = 0; i < count; i++) {
        char student_info[MAX_STRING];
        snprintf(student_info, MAX_STRING, "Student ID: %s\nName: %s\nAttendance: %.2f%%\n\n",
                students[i].id, students[i].name, students[i].courses[0].attendance_percentage);
        strcat(report_content, student_info);
    }

    char report_name[MAX_STRING];
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(report_name, MAX_STRING, "attendance_summary_%Y%m%d%H%M%S", tm_info);

    return file_handler_generate_report(report_name, report_content, format);
}

// Generate a detailed report for a specific student
StatusCode reports_generate_student_report(const Student* student, ExportFormat format) {
    if (!student) {
        return ERROR_INVALID_INPUT;
    }

    char report_content[MAX_STRING * MAX_STUDENTS];
    memset(report_content, 0, sizeof(report_content));

    strcat(report_content, "Student Detailed Report\n");
    strcat(report_content, "=======================\n\n");

    char student_info[MAX_STRING];
    snprintf(student_info, MAX_STRING, "Student ID: %s\nName: %s\nDepartment: %s\nGPA: %.2f\nAttendance: %.2f%%\n\n",
            student->id, student->name, student->department, student->current_gpa, student->courses[0].attendance_percentage);
    strcat(report_content, student_info);

    char report_name[MAX_STRING];
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(report_name, MAX_STRING, "student_report_%Y%m%d%H%M%S", tm_info);

    return file_handler_generate_report(report_name, report_content, format);
}

// Generate a user activity log report
StatusCode reports_generate_user_activity_log(const User* users, int count, ExportFormat format) {
    if (!users || count <= 0) {
        return ERROR_INVALID_INPUT;
    }

    char report_content[MAX_STRING * MAX_USERS];
    memset(report_content, 0, sizeof(report_content));

    strcat(report_content, "User Activity Log Report\n");
    strcat(report_content, "========================\n\n");

    for (int i = 0; i < count; i++) {
        char user_info[MAX_STRING];
        snprintf(user_info, MAX_STRING, "Username: %s\nFull Name: %s\nLast Login: %s\n\n",
                users[i].username, users[i].full_name, ctime(&users[i].last_login));
        strcat(report_content, user_info);
    }

    char report_name[MAX_STRING];
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(report_name, MAX_STRING, "user_activity_log_%Y%m%d%H%M%S", tm_info);

    return file_handler_generate_report(report_name, report_content, format);
}

// Generate a course enrollment report
StatusCode reports_generate_course_enrollment(const Student* students, int count, ExportFormat format) {
    if (!students || count <= 0) {
        return ERROR_INVALID_INPUT;
    }

    char report_content[MAX_STRING * MAX_STUDENTS];
    memset(report_content, 0, sizeof(report_content));

    strcat(report_content, "Course Enrollment Report\n");
    strcat(report_content, "========================\n\n");

    for (int i = 0; i < count; i++) {
        char student_info[MAX_STRING];
        snprintf(student_info, MAX_STRING, "Student ID: %s\nName: %s\nEnrolled Courses:\n",
                students[i].id, students[i].name);
        strcat(report_content, student_info);

        for (int j = 0; j < students[i].course_count; j++) {
            char course_info[MAX_STRING];
            snprintf(course_info, MAX_STRING, "  - %s: %s\n",
                    students[i].courses[j].code, students[i].courses[j].name);
            strcat(report_content, course_info);
        }
        strcat(report_content, "\n");
    }

    char report_name[MAX_STRING];
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(report_name, MAX_STRING, "course_enrollment_%Y%m%d%H%M%S", tm_info);

    return file_handler_generate_report(report_name, report_content, format);
}

// Get error message for status code
const char* reports_get_error_message(StatusCode code) {
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
