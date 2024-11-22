// student.h
#ifndef STUDENT_H
#define STUDENT_H

#include "constants.h"
#include <time.h>

// Course structure to store course-specific information
typedef struct {
    char code[MAX_STRING];          // Course code (e.g., "CS101")
    char name[MAX_STRING];          // Course name
    int credits;                    // Course credit hours
    GradeScale grade;              // Grade received
    float attendance_percentage;    // Attendance percentage
    time_t last_attendance;         // Timestamp of last attendance
} Course;

// Student structure to store student information
typedef struct {
    char id[MAX_STRING];           // Student ID (unique identifier)
    char name[MAX_STRING];         // Student's full name
    char department[MAX_STRING];   // Department name
    char email[MAX_STRING];        // Student's email address
    char phone[MAX_STRING];        // Contact number
    
    // Academic information
    Course courses[MAX_COURSES];   // Array of courses
    int course_count;              // Number of courses enrolled
    float current_gpa;             // Current GPA
    float cgpa;                    // Cumulative GPA
    
    // Administrative information
    time_t admission_date;         // Date of admission
    time_t last_modified;          // Last modification timestamp
    char modified_by[MAX_STRING];  // Username who last modified the record
    int active;                    // Whether the student is currently enrolled
    Version version;               // Version information for this record
} Student;

// Core student management functions
StatusCode student_init(void);
StatusCode student_cleanup(void);

// CRUD operations
StatusCode student_add(const Student* student);
StatusCode student_update(const char* id, const Student* updated_data);
StatusCode student_delete(const char* id);
StatusCode student_get(const char* id, Student* result);
StatusCode student_list(Student* result, int* count);

// Course management
StatusCode student_add_course(const char* student_id, const Course* course);
StatusCode student_update_course(const char* student_id, const char* course_code, const Course* updated_course);
StatusCode student_remove_course(const char* student_id, const char* course_code);
StatusCode student_get_courses(const char* student_id, Course* courses, int* count);

// Academic operations
StatusCode student_update_attendance(const char* student_id, const char* course_code, float attendance);
StatusCode student_update_grade(const char* student_id, const char* course_code, GradeScale grade);
StatusCode student_calculate_gpa(const char* student_id);
StatusCode student_get_academic_status(const char* student_id, char* status);

// Search operations
StatusCode student_search_by_name(const char* name, Student* results, int* count);
StatusCode student_search_by_department(const char* department, Student* results, int* count);
StatusCode student_search_by_gpa_range(float min_gpa, float max_gpa, Student* results, int* count);

// Validation functions
StatusCode student_validate_id(const char* id);
StatusCode student_validate_email(const char* email);
StatusCode student_validate_phone(const char* phone);

// Utility functions
void student_init_structure(Student* student);
char* student_grade_to_string(GradeScale grade);
float student_grade_to_gpa(GradeScale grade);
const char* student_get_error_message(StatusCode code);

#endif // STUDENT_H
