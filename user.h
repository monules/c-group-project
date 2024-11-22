// user.h
#ifndef USER_H
#define USER_H

#include "constants.h"
#include <time.h>

// Session structure to track user login status
typedef struct {
    char session_id[MAX_STRING];
    char username[MAX_STRING];
    UserRole role;
    time_t login_time;
    time_t last_activity;
    int is_active;
} UserSession;

// User structure to store user information
typedef struct {
    char username[MAX_STRING];
    char password[MAX_STRING];      // In production, store hashed password
    char full_name[MAX_STRING];
    char email[MAX_STRING];
    UserRole role;
    ThemeType theme_preference;
    time_t created_at;
    time_t last_login;
    time_t last_password_change;
    int login_attempts;
    int is_locked;                  // Account lockout after too many failed attempts
    char student_id[MAX_STRING];    // Only for student users
    int is_active;                  // Soft delete flag
} User;

// System initialization and cleanup
StatusCode user_init(void);
StatusCode user_cleanup(void);

// User management functions
StatusCode user_create(const User* user);
StatusCode user_update(const char* username, const User* updated_data);
StatusCode user_delete(const char* username);
StatusCode user_get(const char* username, User* result);
StatusCode user_list(User* result, int* count);
StatusCode user_change_password(const char* username, const char* old_password, const char* new_password);
StatusCode user_reset_password(const char* username);  // Admin function

// Authentication and session management
StatusCode user_login(const char* username, const char* password, UserSession* session);
StatusCode user_logout(const char* session_id);
StatusCode user_verify_session(const char* session_id, UserSession* session);
StatusCode user_refresh_session(const char* session_id);
StatusCode user_list_active_sessions(UserSession* sessions, int* count);

// User preferences
StatusCode user_set_theme(const char* username, ThemeType theme);
StatusCode user_get_theme(const char* username, ThemeType* theme);

// Permission management
StatusCode user_has_permission(const char* username, const char* action);
StatusCode user_get_role(const char* username, UserRole* role);
StatusCode user_set_role(const char* username, UserRole new_role);

// Account management
StatusCode user_lock_account(const char* username);
StatusCode user_unlock_account(const char* username);
StatusCode user_increment_login_attempts(const char* username);
StatusCode user_reset_login_attempts(const char* username);

// Validation functions
StatusCode user_validate_username(const char* username);
StatusCode user_validate_password(const char* password);
StatusCode user_validate_email(const char* email);

// Utility functions
const char* user_role_to_string(UserRole role);
UserRole user_string_to_role(const char* role_str);
void user_init_structure(User* user);

// Error message function
const char* user_get_error_message(StatusCode code);

#endif // USER_H
