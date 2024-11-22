#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "constants.h"
#include "student.h"
#include "user.h"

// File paths
#define STUDENT_DATA_FILE "data/students.dat"
#define USER_DATA_FILE "data/users.dat"
#define BACKUP_DIR "data/backups/"
#define LOG_FILE "data/system.log"
#define REPORT_DIR "data/reports/"

// Function prototypes

// Student file operations
StatusCode file_handler_load_students(Student* students, int* count);
StatusCode file_handler_save_students(const Student* students, int count);
StatusCode file_handler_backup_students(void);
StatusCode file_handler_restore_students(Version version);

// User file operations
StatusCode file_handler_load_users(User* users, int* count);
StatusCode file_handler_save_users(const User* users, int count);
StatusCode file_handler_backup_users(void);
StatusCode file_handler_restore_users(Version version);

// Log file operations
StatusCode file_handler_log_action(const char* username, const char* action);
StatusCode file_handler_read_log(char* log_buffer, int buffer_size);

// Report file operations
StatusCode file_handler_generate_report(const char* report_name, const char* content, ExportFormat format);
StatusCode file_handler_export_report(const char* report_name, ExportFormat format);

// Utility functions
const char* file_handler_get_error_message(StatusCode code);

#endif // FILE_HANDLER_H
