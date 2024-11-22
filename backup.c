#include "backup.h"
#include "file_handler.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// Create a backup of the student data
StatusCode backup_create_student_backup(void) {
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

// Restore student data from a backup
StatusCode backup_restore_student_backup(Version version) {
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

// Create a backup of the user data
StatusCode backup_create_user_backup(void) {
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

// Restore user data from a backup
StatusCode backup_restore_user_backup(Version version) {
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

// Get error message for status code
const char* backup_get_error_message(StatusCode code) {
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
