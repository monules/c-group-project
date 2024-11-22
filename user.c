#include "user.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

// Static variables for internal user management
static User* users = NULL;
static int total_users = 0;
static UserSession* active_sessions = NULL;
static int total_sessions = 0;
static int is_initialized = 0;

// Constants for password policy
#define MIN_PASSWORD_LENGTH 8
#define MAX_PASSWORD_LENGTH 64
#define MAX_LOGIN_ATTEMPTS 5
#define SESSION_DURATION 3600  // 1 hour in seconds
#define MAX_SESSIONS 100

// Initialize the user management system
StatusCode user_init(void) {
    if (is_initialized) {
        return SUCCESS;
    }

    users = (User*)malloc(sizeof(User) * MAX_USERS);
    active_sessions = (UserSession*)malloc(sizeof(UserSession) * MAX_SESSIONS);
    
    if (!users || !active_sessions) {
        free(users);
        free(active_sessions);
        return ERROR_MEMORY_ALLOCATION;
    }

    total_users = 0;
    total_sessions = 0;
    is_initialized = 1;

    // Create default admin user if no users exist
    User admin;
    user_init_structure(&admin);
    strncpy(admin.username, "admin", MAX_STRING);
    strncpy(admin.password, "admin123", MAX_STRING);  // In production, use hashed password
    strncpy(admin.full_name, "System Administrator", MAX_STRING);
    admin.role = ROLE_ADMIN;
    
    return user_create(&admin);
}

// Clean up allocated resources
StatusCode user_cleanup(void) {
    if (!is_initialized) {
        return SUCCESS;
    }

    free(users);
    free(active_sessions);
    users = NULL;
    active_sessions = NULL;
    total_users = 0;
    total_sessions = 0;
    is_initialized = 0;
    return SUCCESS;
}

// Initialize a user structure with default values
void user_init_structure(User* user) {
    memset(user, 0, sizeof(User));
    user->theme_preference = THEME_LIGHT;
    user->created_at = time(NULL);
    user->last_password_change = time(NULL);
    user->is_active = 1;
    user->role = ROLE_STUDENT;  // Default role
}

// Validate username format
StatusCode user_validate_username(const char* username) {
    if (!username || strlen(username) < 3 || strlen(username) > MAX_STRING) {
        return ERROR_INVALID_INPUT;
    }

    // Username should contain only alphanumeric characters and underscore
    for (int i = 0; username[i]; i++) {
        if (!isalnum(username[i]) && username[i] != '_') {
            return ERROR_INVALID_INPUT;
        }
    }
    return SUCCESS;
}

// Validate password strength
StatusCode user_validate_password(const char* password) {
    if (!password || strlen(password) < MIN_PASSWORD_LENGTH || 
        strlen(password) > MAX_PASSWORD_LENGTH) {
        return ERROR_INVALID_INPUT;
    }

    int has_upper = 0, has_lower = 0, has_digit = 0, has_special = 0;
    
    for (int i = 0; password[i]; i++) {
        if (isupper(password[i])) has_upper = 1;
        else if (islower(password[i])) has_lower = 1;
        else if (isdigit(password[i])) has_digit = 1;
        else has_special = 1;
    }

    if (!(has_upper && has_lower && has_digit)) {
        return ERROR_INVALID_INPUT;
    }

    return SUCCESS;
}

// Generate a random session ID
static void generate_session_id(char* session_id) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int charset_size = sizeof(charset) - 1;
    
    for (int i = 0; i < 32; i++) {
        session_id[i] = charset[rand() % charset_size];
    }
    session_id[32] = '\0';
}

// Create a new user
StatusCode user_create(const User* user) {
    if (!is_initialized || !user) {
        return ERROR_INVALID_INPUT;
    }

    if (total_users >= MAX_USERS) {
        return ERROR_DATABASE_FULL;
    }

    // Validate user data
    StatusCode status;
    if ((status = user_validate_username(user->username)) != SUCCESS) {
        return status;
    }
    if ((status = user_validate_password(user->password)) != SUCCESS) {
        return status;
    }

    // Check for duplicate username
    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, user->username) == 0) {
            return ERROR_INVALID_INPUT;
        }
    }

    // Add the user
    users[total_users] = *user;
    total_users++;
    return SUCCESS;
}

// User login
StatusCode user_login(const char* username, const char* password, UserSession* session) {
    if (!is_initialized || !username || !password || !session) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            if (users[i].is_locked) {
                return ERROR_PERMISSION_DENIED;
            }

            if (strcmp(users[i].password, password) == 0) {
                // Success - create new session
                generate_session_id(session->session_id);
                strncpy(session->username, username, MAX_STRING);
                session->role = users[i].role;
                session->login_time = time(NULL);
                session->last_activity = time(NULL);
                session->is_active = 1;

                // Update user login info
                users[i].last_login = time(NULL);
                users[i].login_attempts = 0;

                // Add to active sessions
                if (total_sessions < MAX_SESSIONS) {
                    active_sessions[total_sessions++] = *session;
                    return SUCCESS;
                }
                return ERROR_DATABASE_FULL;
            }

            // Failed login attempt
            users[i].login_attempts++;
            if (users[i].login_attempts >= MAX_LOGIN_ATTEMPTS) {
                users[i].is_locked = 1;
                return ERROR_PERMISSION_DENIED;
            }
            return ERROR_INVALID_CREDENTIALS;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// Verify session validity
StatusCode user_verify_session(const char* session_id, UserSession* session) {
    if (!is_initialized || !session_id) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_sessions; i++) {
        if (strcmp(active_sessions[i].session_id, session_id) == 0) {
            if (!active_sessions[i].is_active) {
                return ERROR_SESSION_EXPIRED;
            }

            time_t current_time = time(NULL);
            if (current_time - active_sessions[i].last_activity > SESSION_DURATION) {
                active_sessions[i].is_active = 0;
                return ERROR_SESSION_EXPIRED;
            }

            active_sessions[i].last_activity = current_time;
            if (session) {
                *session = active_sessions[i];
            }
            return SUCCESS;
        }
    }
    return ERROR_INVALID_CREDENTIALS;
}

// User logout
StatusCode user_logout(const char* session_id) {
    if (!is_initialized || !session_id) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_sessions; i++) {
        if (strcmp(active_sessions[i].session_id, session_id) == 0) {
            active_sessions[i].is_active = 0;
            return SUCCESS;
        }
    }
    return ERROR_INVALID_CREDENTIALS;
}

// Check if user has permission for an action
StatusCode user_has_permission(const char* username, const char* action) {
    if (!is_initialized || !username || !action) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            // Basic role-based permissions
            if (users[i].role == ROLE_ADMIN) {
                return SUCCESS;  // Admin has all permissions
            }
            
            if (users[i].role == ROLE_PROFESSOR) {
                // Professor permissions
                if (strcmp(action, "view_student") == 0 ||
                    strcmp(action, "edit_grade") == 0 ||
                    strcmp(action, "view_attendance") == 0) {
                    return SUCCESS;
                }
            }
            
            if (users[i].role == ROLE_STUDENT) {
                // Student permissions
                if (strcmp(action, "view_own_grade") == 0 ||
                    strcmp(action, "view_own_attendance") == 0) {
                    return SUCCESS;
                }
            }
            
            return ERROR_PERMISSION_DENIED;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// Convert UserRole to string
const char* user_role_to_string(UserRole role) {
    static const char* role_strings[] = {
        [ROLE_ADMIN] = "Administrator",
        [ROLE_PROFESSOR] = "Professor",
        [ROLE_STUDENT] = "Student"
    };

    if (role >= ROLE_ADMIN && role <= ROLE_STUDENT) {
        return role_strings[role];
    }
    return "Unknown";
}

// Convert string to UserRole
UserRole user_string_to_role(const char* role_str) {
    if (!role_str) return ROLE_STUDENT;  // Default role

    if (strcmp(role_str, "Administrator") == 0) return ROLE_ADMIN;
    if (strcmp(role_str, "Professor") == 0) return ROLE_PROFESSOR;
    return ROLE_STUDENT;
}

// Update user information
StatusCode user_update(const char* username, const User* updated_data) {
    if (!is_initialized || !username || !updated_data) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            User temp = users[i];
            users[i] = *updated_data;
            
            // Preserve certain fields
            users[i].created_at = temp.created_at;
            users[i].login_attempts = temp.login_attempts;
            users[i].is_locked = temp.is_locked;
            
            return SUCCESS;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// Set user theme preference
StatusCode user_set_theme(const char* username, ThemeType theme) {
    if (!is_initialized || !username) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            users[i].theme_preference = theme;
            return SUCCESS;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// List all active sessions
StatusCode user_list_active_sessions(UserSession* sessions, int* count) {
    if (!is_initialized || !sessions || !count) {
        return ERROR_INVALID_INPUT;
    }

    *count = 0;
    time_t current_time = time(NULL);

    for (int i = 0; i < total_sessions; i++) {
        if (active_sessions[i].is_active &&
            (current_time - active_sessions[i].last_activity <= SESSION_DURATION)) {
            sessions[(*count)++] = active_sessions[i];
        }
    }
    return SUCCESS;
}

// Delete a user record
StatusCode user_delete(const char* username) {
    if (!is_initialized || !username) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            // Soft delete by setting is_active to 0
            users[i].is_active = 0;
            return SUCCESS;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// Retrieve a user record
StatusCode user_get(const char* username, User* result) {
    if (!is_initialized || !username || !result) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            *result = users[i];
            return SUCCESS;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// List all users
StatusCode user_list(User* result, int* count) {
    if (!is_initialized || !result || !count) {
        return ERROR_INVALID_INPUT;
    }

    *count = 0;
    for (int i = 0; i < total_users; i++) {
        if (users[i].is_active) {
            result[(*count)++] = users[i];
        }
    }
    return SUCCESS;
}

// Change user password
StatusCode user_change_password(const char* username, const char* old_password, const char* new_password) {
    if (!is_initialized || !username || !old_password || !new_password) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            if (strcmp(users[i].password, old_password) == 0) {
                StatusCode status = user_validate_password(new_password);
                if (status != SUCCESS) {
                    return status;
                }
                strncpy(users[i].password, new_password, MAX_STRING);
                users[i].last_password_change = time(NULL);
                return SUCCESS;
            }
            return ERROR_INVALID_CREDENTIALS;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// Reset user password (admin function)
StatusCode user_reset_password(const char* username) {
    if (!is_initialized || !username) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            // Set default password in production, use secure password
            strncpy(users[i].password, "default123", MAX_STRING);
            users[i].last_password_change = time(NULL);
            return SUCCESS;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// Refresh user session
StatusCode user_refresh_session(const char* session_id) {
    if (!is_initialized || !session_id) {
        return ERROR_INVALID_INPUT;
    }

    time_t current_time = time(NULL);

    for (int i = 0; i < total_sessions; i++) {
        if (strcmp(active_sessions[i].session_id, session_id) == 0) {
            if (!active_sessions[i].is_active) {
                return ERROR_SESSION_EXPIRED;
            }

            if (current_time - active_sessions[i].last_activity > SESSION_DURATION) {
                active_sessions[i].is_active = 0;
                return ERROR_SESSION_EXPIRED;
            }

            active_sessions[i].last_activity = current_time;
            return SUCCESS;
        }
    }
    return ERROR_INVALID_CREDENTIALS;
}
// Validate email format
StatusCode user_validate_email(const char* email) {
    if (!email || strlen(email) < 5 || strlen(email) > MAX_STRING) {
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

// Retrieve the theme preference for a user
StatusCode user_get_theme(const char* username, ThemeType* theme) {
    if (!is_initialized || !username || !theme) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            *theme = users[i].theme_preference;
            return SUCCESS;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// Retrieve the role of a user
StatusCode user_get_role(const char* username, UserRole* role) {
    if (!is_initialized || !username || !role) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            *role = users[i].role;
            return SUCCESS;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// Set a new role for a user
StatusCode user_set_role(const char* username, UserRole new_role) {
    if (!is_initialized || !username) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            users[i].role = new_role;
            return SUCCESS;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// Lock a user account
StatusCode user_lock_account(const char* username) {
    if (!is_initialized || !username) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            users[i].is_locked = 1;
            return SUCCESS;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// Unlock a user account
StatusCode user_unlock_account(const char* username) {
    if (!is_initialized || !username) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            users[i].is_locked = 0;
            return SUCCESS;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// Increment the login attempts counter for a user
StatusCode user_increment_login_attempts(const char* username) {
    if (!is_initialized || !username) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            users[i].login_attempts++;
            return SUCCESS;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// Reset the login attempts counter for a user
StatusCode user_reset_login_attempts(const char* username) {
    if (!is_initialized || !username) {
        return ERROR_INVALID_INPUT;
    }

    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            users[i].login_attempts = 0;
            return SUCCESS;
        }
    }
    return ERROR_USER_NOT_FOUND;
}

// user_get_error_message function
const char* user_get_error_message(StatusCode code) {
    static const char* messages[] = {
        [SUCCESS] = "Operation completed successfully",
        [ERROR_FILE_NOT_FOUND] = "File not found",
        [ERROR_PERMISSION_DENIED] = "Permission denied",
        [ERROR_INVALID_INPUT] = "Invalid input provided",
        [ERROR_MEMORY_ALLOCATION] = "Memory allocation failed",
        [ERROR_DATABASE_FULL] = "Database is full",
        [ERROR_USER_NOT_FOUND] = "User not found",
        [ERROR_INVALID_CREDENTIALS] = "Invalid username or password",
        [ERROR_SESSION_EXPIRED] = "Session has expired",
    };

    if (code >= 0 && code < sizeof(messages) / sizeof(messages[0])) {
        return messages[code];
    }

    return "Unknown error";
}
