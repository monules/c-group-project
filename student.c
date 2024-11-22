// student.c
#include "student.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

// Static variables for internal student management
static Student* students = NULL;
static int total_students = 0;
static int is_initialized = 0;

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

// Initialize the student management system
StatusCode student_init(void) {
    if (is_initialized) {
        return SUCCESS;
    }

    students = (Student*)malloc(sizeof(Student) * MAX_STUDENTS);
    if (students == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }

    total_students = 0;
    is_initialized = 1;
    return SUCCESS;
}

// Clean up allocated resources
StatusCode student_cleanup(void) {
    if (!is_initialized) {
        return SUCCESS;
    }

    free(students);
    students = NULL;
    total_students = 0;
    is_initialized = 0;
    return SUCCESS;
}

// Initialize a student structure with default values
void student_init_structure(Student* student) {
    memset(student, 0, sizeof(Student));
    student->active = 1;
    student->version.major = 1;
    student->version.minor = 0;
    student->version.patch = 0;
    student->version.timestamp = time(NULL);
    student->admission_date = time(NULL);
    student->last_modified = time(NULL);
}

// Validate student ID format
StatusCode student_validate_id(const char* id) {
    if (id == NULL || strlen(id) < 5 || strlen(id) > MAX_STRING) {
        return ERROR_INVALID_INPUT;
    }

    // Check ID format (example: assumes format like "2024001")
    for (int i = 0; id[i]; i++) {
        if (!isdigit(id[i])) {
            return ERROR_INVALID_INPUT;
        }
    }
    return SUCCESS;
}

// Validate email format
StatusCode student_validate_email(const char* email) {
    if (email == NULL || strlen(email) < 5 || strlen(email) > MAX_STRING) {
        return ERROR_INVALID_INPUT;
    }

    // Basic email validation (contains @ and .)
    const char* at = strchr(email, '@');
    const char* dot = strrchr(email, '.');
    if (!at || !dot || dot < at) {
        return ERROR_INVALID_INPUT;
    }
    return SUCCESS;
}

// Validate phone number format
StatusCode student_validate_phone(const char* phone) {
    if (phone == NULL || strlen(phone) < 10 || strlen(phone) > MAX_STRING) {
        return ERROR_INVALID_INPUT;
    }

    // Check phone format (allows digits and optional '+' at start)
    if (phone[0] != '+' && !isdigit(phone[0])) {
        return ERROR_INVALID_INPUT;
    }
    for (int i = 1; phone[i]; i++) {
        if (!isdigit(phone[i])) {
            return ERROR_INVALID_INPUT;
        }
    }
    return SUCCESS;
}

// Add a new student
StatusCode student_add(const Student* student) {
    if (!is_initialized) {
        return ERROR_INVALID_INPUT;
    }

    if (total_students >= MAX_STUDENTS) {
        return ERROR_DATABASE_FULL;
    }

    // Validate student data
    StatusCode status;
    if ((status = student_validate_id(student->id)) != SUCCESS) {
        return status;
    }
    if ((status = student_validate_email(student->email)) != SUCCESS) {
        return status;
    }
    if ((status = student_validate_phone(student->phone)) != SUCCESS) {
        return status;
    }

    // Check for duplicate ID
    for (int i = 0; i < total_students; i++) {
        if (strcmp(students[i].id, student->id) == 0) {
            return ERROR_INVALID_INPUT;
        }
    }

    // Add the student
    students[total_students] = *student;
    total_students++;
    return SUCCESS;
}

// Update existing student information
StatusCode student_update(const char* id, const Student* updated_data) {
    if (!is_initialized || !id || !updated_data) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_students; i++) {
        if (strcmp(students[i].id, id) == 0) {
            // Preserve some original data
            time_t original_admission = students[i].admission_date;
            Version original_version = students[i].version;

            // Update student data
            students[i] = *updated_data;
            
            // Restore preserved data
            students[i].admission_date = original_admission;
            students[i].version = original_version;
            students[i].version.patch++;
            students[i].version.timestamp = time(NULL);
            students[i].last_modified = time(NULL);
            
            return SUCCESS;
        }
    }
    return ERROR_STUDENT_NOT_FOUND;
}

// Delete a student record
StatusCode student_delete(const char* id) {
    if (!is_initialized || !id) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_students; i++) {
        if (strcmp(students[i].id, id) == 0) {
            // Shift remaining students
            for (int j = i; j < total_students - 1; j++) {
                students[j] = students[j + 1];
            }
            total_students--;
            return SUCCESS;
        }
    }
    return ERROR_STUDENT_NOT_FOUND;
}

// Retrieve a student record
StatusCode student_get(const char* id, Student* result) {
    if (!is_initialized || !id || !result) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_students; i++) {
        if (strcmp(students[i].id, id) == 0) {
            *result = students[i];
            return SUCCESS;
        }
    }
    return ERROR_STUDENT_NOT_FOUND;
}

// List all students
StatusCode student_list(Student* result, int* count) {
    if (!is_initialized || !result || !count) {
        return ERROR_INVALID_INPUT;
    }

    *count = total_students;
    memcpy(result, students, sizeof(Student) * total_students);
    return SUCCESS;
}

// Add a course to student's record
StatusCode student_add_course(const char* student_id, const Course* course) {
    if (!is_initialized || !student_id || !course) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_students; i++) {
        if (strcmp(students[i].id, student_id) == 0) {
            if (students[i].course_count >= MAX_COURSES) {
                return ERROR_DATABASE_FULL;
            }

            // Check for duplicate course
            for (int j = 0; j < students[i].course_count; j++) {
                if (strcmp(students[i].courses[j].code, course->code) == 0) {
                    return ERROR_INVALID_INPUT;
                }
            }

            students[i].courses[students[i].course_count] = *course;
            students[i].course_count++;
            students[i].last_modified = time(NULL);
            
            // Recalculate GPA
            return student_calculate_gpa(student_id);
        }
    }
    return ERROR_STUDENT_NOT_FOUND;
}

// Calculate GPA for a student
StatusCode student_calculate_gpa(const char* student_id) {
    if (!is_initialized || !student_id) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_students; i++) {
        if (strcmp(students[i].id, student_id) == 0) {
            float total_points = 0.0f;
            int total_credits = 0;

            for (int j = 0; j < students[i].course_count; j++) {
                Course* course = &students[i].courses[j];
                float grade_point = grade_points[course->grade];
                total_points += grade_point * course->credits;
                total_credits += course->credits;
            }

            if (total_credits > 0) {
                students[i].current_gpa = total_points / total_credits;
            } else {
                students[i].current_gpa = 0.0f;
            }
            return SUCCESS;
        }
    }
    return ERROR_STUDENT_NOT_FOUND;
}

// Convert grade to string representation
char* student_grade_to_string(GradeScale grade) {
    static char* grade_strings[] = {
        "A+", "A", "A-", "B+", "B", "B-",
        "C+", "C", "C-", "D+", "D", "F"
    };

    if (grade >= GRADE_A_PLUS && grade <= GRADE_F) {
        return grade_strings[grade];
    }
    return "Invalid";
}

// Convert grade to GPA value
float student_grade_to_gpa(GradeScale grade) {
    if (grade >= GRADE_A_PLUS && grade <= GRADE_F) {
        return grade_points[grade];
    }
    return 0.0f;
}

// Search students by name
StatusCode student_search_by_name(const char* name, Student* results, int* count) {
    if (!is_initialized || !name || !results || !count) {
        return ERROR_INVALID_INPUT;
    }

    *count = 0;
    for (int i = 0; i < total_students; i++) {
        if (strstr(students[i].name, name) != NULL) {
            results[*count] = students[i];
            (*count)++;
        }
    }
    return SUCCESS;
}

// Get error message for status code
const char* student_get_error_message(StatusCode code) {
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
