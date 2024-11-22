#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Load student data from file
StatusCode file_handler_load_students(Student* students, int* count) {
    FILE* file = fopen(STUDENT_DATA_FILE, "rb");
    if (!file) {
        return ERROR_FILE_NOT_FOUND;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size % sizeof(Student) != 0) {
        fclose(file);
        return ERROR_INVALID_INPUT;
    }

    *count = file_size / sizeof(Student);
    fread(students, sizeof(Student), *count, file);
    fclose(file);
    return SUCCESS;
}

// Save student data to file
StatusCode file_handler_save_students(const Student* students, int count) {
    FILE* file = fopen(STUDENT_DATA_FILE, "wb");
    if (!file) {
        return ERROR_FILE_NOT_FOUND;
    }

    fwrite(students, sizeof(Student), count, file);
    fclose(file);
    return SUCCESS;
}

// Backup student data
StatusCode file_handler_backup_students(void) {
    char backup_file[MAX_STRING];
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(backup_file, MAX_STRING, "%Y%m%d%H%M%S_students.dat", tm_info);

    FILE* src = fopen(STUDENT_DATA_FILE, "rb");
    if (!src) {
        return ERROR_FILE_NOT_FOUND;
    }

    char full_path[MAX_STRING];
    snprintf(full_path, MAX_STRING, "%s%s", BACKUP_DIR, backup_file);

    FILE* dest = fopen(full_path, "wb");
    if (!dest) {
        fclose(src);
        return ERROR_FILE_NOT_FOUND;
    }

    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes_read, dest);
    }

    fclose(src);
    fclose(dest);
    return SUCCESS;
}

// Restore student data from a specific version
StatusCode file_handler_restore_students(Version version) {
    char backup_file[MAX_STRING];
    snprintf(backup_file, MAX_STRING, "%s%d.%d.%d_students.dat", BACKUP_DIR, version.major, version.minor, version.patch);

    FILE* src = fopen(backup_file, "rb");
    if (!src) {
        return ERROR_FILE_NOT_FOUND;
    }

    FILE* dest = fopen(STUDENT_DATA_FILE, "wb");
    if (!dest) {
        fclose(src);
        return ERROR_FILE_NOT_FOUND;
    }

    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes_read, dest);
    }

    fclose(src);
    fclose(dest);
    return SUCCESS;
}

// Load user data from file
StatusCode file_handler_load_users(User* users, int* count) {
    FILE* file = fopen(USER_DATA_FILE, "rb");
    if (!file) {
        return ERROR_FILE_NOT_FOUND;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size % sizeof(User) != 0) {
        fclose(file);
        return ERROR_INVALID_INPUT;
    }

    *count = file_size / sizeof(User);
    fread(users, sizeof(User), *count, file);
    fclose(file);
    return SUCCESS;
}

// Save user data to file
StatusCode file_handler_save_users(const User* users, int count) {
    FILE* file = fopen(USER_DATA_FILE, "wb");
    if (!file) {
        return ERROR_FILE_NOT_FOUND;
    }

    fwrite(users, sizeof(User), count, file);
    fclose(file);
    return SUCCESS;
}

// Backup user data
StatusCode file_handler_backup_users(void) {
    char backup_file[MAX_STRING];
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(backup_file, MAX_STRING, "%Y%m%d%H%M%S_users.dat", tm_info);

    FILE* src = fopen(USER_DATA_FILE, "rb");
    if (!src) {
        return ERROR_FILE_NOT_FOUND;
    }

    char full_path[MAX_STRING];
    snprintf(full_path, MAX_STRING, "%s%s", BACKUP_DIR, backup_file);

    FILE* dest = fopen(full_path, "wb");
    if (!dest) {
        fclose(src);
        return ERROR_FILE_NOT_FOUND;
    }

    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes_read, dest);
    }

    fclose(src);
    fclose(dest);
    return SUCCESS;
}

// Restore user data from a specific version
StatusCode file_handler_restore_users(Version version) {
    char backup_file[MAX_STRING];
    snprintf(backup_file, MAX_STRING, "%s%d.%d.%d_users.dat", BACKUP_DIR, version.major, version.minor, version.patch);

    FILE* src = fopen(backup_file, "rb");
    if (!src) {
        return ERROR_FILE_NOT_FOUND;
    }

    FILE* dest = fopen(USER_DATA_FILE, "wb");
    if (!dest) {
        fclose(src);
        return ERROR_FILE_NOT_FOUND;
    }

    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes_read, dest);
    }

    fclose(src);
    fclose(dest);
    return SUCCESS;
}

// Log a user action to the system log file
StatusCode file_handler_log_action(const char* username, const char* action) {
    FILE* file = fopen(LOG_FILE, "a");
    if (!file) {
        return ERROR_FILE_NOT_FOUND;
    }

    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char timestamp[MAX_STRING];
    strftime(timestamp, MAX_STRING, "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(file, "[%s] %s: %s\n", timestamp, username, action);
    fclose(file);
    return SUCCESS;
}

// Read the system log file into a buffer
StatusCode file_handler_read_log(char* log_buffer, int buffer_size) {
    FILE* file = fopen(LOG_FILE, "r");
    if (!file) {
        return ERROR_FILE_NOT_FOUND;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size > buffer_size) {
        fclose(file);
        return ERROR_INVALID_INPUT;
    }

    fread(log_buffer, 1, file_size, file);
    log_buffer[file_size] = '\0';
    fclose(file);
    return SUCCESS;
}

// Generate a report and save it to a file
StatusCode file_handler_generate_report(const char* report_name, const char* content, ExportFormat format) {
    char report_file[MAX_STRING];
    snprintf(report_file, MAX_STRING, "%s%s", REPORT_DIR, report_name);

    FILE* file = fopen(report_file, "w");
    if (!file) {
        return ERROR_FILE_NOT_FOUND;
    }

    fprintf(file, "%s", content);
    fclose(file);
    return SUCCESS;
}

// Export a report in the specified format
StatusCode file_handler_export_report(const char* report_name, ExportFormat format) {
    char report_file[MAX_STRING];
    snprintf(report_file, MAX_STRING, "%s%s", REPORT_DIR, report_name);

    FILE* src = fopen(report_file, "r");
    if (!src) {
        return ERROR_FILE_NOT_FOUND;
    }

    char export_file[MAX_STRING];
    switch (format) {
        case FORMAT_TEXT:
            snprintf(export_file, MAX_STRING, "%s%s.txt", REPORT_DIR, report_name);
            break;
        case FORMAT_CSV:
            snprintf(export_file, MAX_STRING, "%s%s.csv", REPORT_DIR, report_name);
            break;
        case FORMAT_JSON:
            snprintf(export_file, MAX_STRING, "%s%s.json", REPORT_DIR, report_name);
            break;
        default:
            fclose(src);
            return ERROR_INVALID_INPUT;
    }

    FILE* dest = fopen(export_file, "w");
    if (!dest) {
        fclose(src);
        return ERROR_FILE_NOT_FOUND;
    }

    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes_read, dest);
    }

    fclose(src);
    fclose(dest);
    return SUCCESS;
}

// Retrieve an error message for a given status code
const char* file_handler_get_error_message(StatusCode code) {
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
