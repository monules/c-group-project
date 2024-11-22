#ifndef BACKUP_H
#define BACKUP_H

#include "constants.h"
#include "student.h"
#include "user.h"

// Function prototypes

// Create a backup of the student data
StatusCode backup_create_student_backup(void);

// Restore student data from a backup
StatusCode backup_restore_student_backup(Version version);

// Create a backup of the user data
StatusCode backup_create_user_backup(void);

// Restore user data from a backup
StatusCode backup_restore_user_backup(Version version);

// Utility function to get the error message for a status code
const char* backup_get_error_message(StatusCode code);

#endif // BACKUP_H
