// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

// System limits
#define MAX_STRING 256
#define MAX_STUDENTS 1000
#define MAX_COURSES 50
#define MAX_USERS 100
#define MAX_GRADES 100

// File paths
#define STUDENT_DATA_FILE "data/students.dat"
#define USER_DATA_FILE "data/users.dat"
#define BACKUP_DIR "data/backups/"
#define LOG_FILE "data/system.log"
#define REPORT_DIR "data/reports/"

// Time constants
#define BACKUP_INTERVAL 300 // 5 minutes in seconds
#define SESSION_TIMEOUT 1800 // 30 minutes in seconds

// Status codes for error handling
typedef enum {
    SUCCESS = 0,
    ERROR_FILE_NOT_FOUND,
    ERROR_PERMISSION_DENIED,
    ERROR_INVALID_INPUT,
    ERROR_MEMORY_ALLOCATION,
    ERROR_DATABASE_FULL,
    ERROR_USER_NOT_FOUND,
    ERROR_STUDENT_NOT_FOUND,
    ERROR_ENCRYPTION_FAILED,
    ERROR_DECRYPTION_FAILED,
    ERROR_INVALID_GRADE,
    ERROR_INVALID_CREDENTIALS,
    ERROR_SESSION_EXPIRED,
    ERROR_BACKUP_FAILED,
    ERROR_RESTORE_FAILED
} StatusCode;

// User roles for permission management
typedef enum {
    ROLE_ADMIN,
    ROLE_PROFESSOR,
    ROLE_STUDENT
} UserRole;

// Theme options
typedef enum {
    THEME_LIGHT,
    THEME_DARK
} ThemeType;

// Export format options
typedef enum {
    FORMAT_TEXT,
    FORMAT_CSV,
    FORMAT_JSON
} ExportFormat;

// Grade scale for GPA calculation
typedef enum {
    GRADE_A_PLUS = 0,  // 4.0
    GRADE_A,           // 4.0
    GRADE_A_MINUS,     // 3.7
    GRADE_B_PLUS,      // 3.3
    GRADE_B,           // 3.0
    GRADE_B_MINUS,     // 2.7
    GRADE_C_PLUS,      // 2.3
    GRADE_C,           // 2.0
    GRADE_C_MINUS,     // 1.7
    GRADE_D_PLUS,      // 1.3
    GRADE_D,           // 1.0
    GRADE_F            // 0.0
} GradeScale;

// Version control
typedef struct {
    int major;     // Major version number
    int minor;     // Minor version number
    int patch;     // Patch number
    long timestamp; // Unix timestamp of the version
} Version;

#endif // CONSTANTS_H
