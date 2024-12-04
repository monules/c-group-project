#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define MAX_LINE 1024
#define MAX_COURSES 500
#define MAX_STUDENTS 1000
#define MAX_PROFESSORS 50
#define MAX_STRING 100

char STUDENTS_FILE[] = "students.txt";
char PROFESSORS_FILE[] = "professors.txt";
char STUDENT_COURSES_FILE[] = "student_classes.txt";
char COURSES_FILE[] = "courses.txt";

typedef struct {
    char courseName[MAX_STRING];
    char courseCode[MAX_STRING];
} Course;

typedef struct {
    char id[MAX_STRING];
    char name[MAX_STRING];
    char faculty[MAX_STRING];
    char advisorName[MAX_STRING];
    char email[MAX_STRING];
    char phoneNumber[MAX_STRING];
    char dateOfBirth[MAX_STRING];
    char gender;
    char address[MAX_STRING];
    int scholarshipAmount; // Percentage (e.g., 100, 50)
    char clubsAttending[MAX_STRING]; // "None" or list of clubs
    char password[MAX_STRING];
    char nationality[MAX_STRING];
    char advisorID[MAX_STRING];
    int semesters;

    char courses[6][MAX_STRING];  // Array to store up to 6 course names
    double grades[6];  
} Student;

typedef struct {
    char id[MAX_STRING], name[MAX_STRING], major[MAX_STRING], email[MAX_STRING], phone[MAX_STRING], password[MAX_STRING], courses[6][MAX_STRING];;
    int courseCount;
} Professor;

// Global arrays to hold data
Course courses[MAX_COURSES];
Student students[MAX_STUDENTS];
Professor professors[MAX_PROFESSORS];

int courseCount = 0, studentCount = 0, professorCount = 0;

void loadStudents();
void loadProfessors();
void loadStudentCourses();
void saveStudents();
void saveProfessors();
void saveStudentCourses();
void mainMenu();
void adminMenu();
void professorMenu(Professor *prof);
void studentMenu(Student *stud);
int login(char *role, char *id, char *password);
void addNewStudent();
void deleteStudent();
void viewInformationAdmin();
void changeInformationAdmin();
void encryptData();
void decryptData();
void changeProfessorInfo(Professor *prof);
void viewStudentsProfessor(Professor *prof);
void generateStudentReport();
void filterStudentsProfessor();
void encryptGradesProfessor();
void viewStudentInfo(Student *stud);
void changeStudentInfo(Student *stud);
void switchFunction();
int validateEmail(char *email);
int validatePhoneNumber(char *phone);
int validatePassword(char *password);
int validateName(char *name);
int validateBirthday(char *birthday);
int validateMajor(char *major);
void encrypt(char *text, int key);
void decrypt(char *text, int key);

int main() {
    loadStudents();
    loadProfessors();
    loadStudentCourses(); // Load student courses and grades
    mainMenu();
    return 0;
}

void mainMenu() {
    int choice;
    char id[MAX_STRING], password[MAX_STRING];
    while (1) {
        printf("\n--- Main Menu ---\n1. Login as Admin\n2. Login as Professor\n3. Login as Student\n4. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline
        if (choice == 4) {
            printf("Exiting...\n");
            break;
        }
        printf("Enter ID: ");
        fgets(id, MAX_STRING, stdin);
        id[strcspn(id, "\n")] = '\0'; // Remove newline
        printf("Enter Password: ");
        fgets(password, MAX_STRING, stdin);
        password[strcspn(password, "\n")] = '\0'; // Remove newline

        if (choice == 1) {
            if (strcmp(id, "admin") == 0 && strcmp(password, "admin123") == 0)
                adminMenu();
            else
                printf("Invalid admin credentials.\n");
        } else if (choice == 2) {
            int idx = login("professor", id, password);
            if (idx != -1)
                professorMenu(&professors[idx]);
            else
                printf("Invalid professor credentials.\n");
        } else if (choice == 3) {
            int idx = login("student", id, password);
            if (idx != -1)
                studentMenu(&students[idx]);
            else
                printf("Invalid student credentials.\n");
        }
        else
            printf("Invalid choice. Try again.\n");
    }
}

void loadStudentCourses() {
    FILE *fp = fopen(STUDENT_COURSES_FILE, "r");
    if (!fp) {
        perror("File opening failed");
        printf("Error opening %s\n", STUDENT_COURSES_FILE);
        return;
    }
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';           // Remove newline character at the end if present
        char *studentID = strtok(line, ":");            // Split the line into student ID, courses, and grades
        char *coursesStr = strtok(NULL, ":");
        char *gradesStr = strtok(NULL, ":");

        if (!studentID || !coursesStr || !gradesStr) {
            printf("Invalid format in %s\n", STUDENT_COURSES_FILE);
            continue;
        }
        int found = 0;  // Find the student by ID
        for (int i = 0; i < studentCount; i++) {
            if (strcmp(students[i].id, studentID) == 0) {
                found = 1;
                char *course = strtok(coursesStr, ",");                 // split or read courses
                int courseIdx = 0;
                while (course && courseIdx < 6) {
                    strcpy(students[i].courses[courseIdx], course);
                    course = strtok(NULL, ",");
                    courseIdx++;
                }
                char *grade = strtok(gradesStr, ",");           //split or read grades
                int gradeIdx = 0;
                while (grade && gradeIdx < 6) {
                    students[i].grades[gradeIdx] = atof(grade);
                    grade = strtok(NULL, ",");
                    gradeIdx++;
                }
                break;
            }
        }

        if (!found) {
            printf("Student ID %s not found in students list.\n", studentID);
        }
    }
    fclose(fp);
}

void loadStudents() {
    FILE *fp = fopen(STUDENTS_FILE, "r");
    if (!fp) {
        perror("File opening failed");
        printf("Error opening %s\n", STUDENTS_FILE);
        return;
    }
    char line[MAX_LINE];
    fgets(line, sizeof(line), fp); // Skip header
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%c\t%s\t%d\t%d\t%s\t%s\t%s\t%s",
            students[studentCount].id,
            students[studentCount].name,
            students[studentCount].faculty,
            students[studentCount].advisorName,
            students[studentCount].email,
            students[studentCount].phoneNumber,
            students[studentCount].dateOfBirth,
            &students[studentCount].gender,
            students[studentCount].address,
            &students[studentCount].semesters,
            &students[studentCount].scholarshipAmount,
            students[studentCount].clubsAttending,
            students[studentCount].password,
            students[studentCount].nationality,
            students[studentCount].advisorID);
        studentCount++;
    }
    fclose(fp);
}

void loadProfessors() {
    FILE *fp = fopen(PROFESSORS_FILE, "r");
    if (!fp) {
        perror("File opening failed");
        printf("Error opening %s\n", PROFESSORS_FILE);
        return;
    }
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        sscanf(token, "ID: %s", professors[professorCount].id);
        token = strtok(NULL, ",");
        sscanf(token, " Name: %[^\n]", professors[professorCount].name);
        token = strtok(NULL, ",");
        sscanf(token, " Major: %s", professors[professorCount].major);
        token = strtok(NULL, ",");
        sscanf(token, " Email: %s", professors[professorCount].email);
        token = strtok(NULL, ",");
        sscanf(token, " Phone: %s", professors[professorCount].phone);
        token = strtok(NULL, ",");
        sscanf(token, " Password: %s", professors[professorCount].password);
        token = strtok(NULL, ",");
        professorCount++;
    }
    fclose(fp);
}

void saveStudents() {
    FILE *fp = fopen(STUDENTS_FILE, "w");
    if (!fp) {
        perror("File opening failed");
        printf("Error opening %s for writing.\n", STUDENTS_FILE);
        return;
    }
    fprintf(fp, "Student ID\tName\tFaculty\tAdvisor's Name\tEmail\tPhone Number\tDate of Birth\tGender\tAddress\tSemesters\tScholarship Amount\tClubs Attending\tPassword\tNationality\tAdvisor's ID\n");
    for (int i = 0; i < studentCount; i++) {
        fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%c\t%s\t%d\t%d\t%s\t%s\t%s\t%s\n",
                students[i].id,
                students[i].name,
                students[i].faculty,
                students[i].advisorName,
                students[i].email,
                students[i].phoneNumber,
                students[i].dateOfBirth,
                students[i].gender, 
                students[i].address,
                students[i].semesters,
                students[i].scholarshipAmount,
                students[i].clubsAttending,
                students[i].password,
                students[i].nationality,
                students[i].advisorID);
    }
    fclose(fp);
}
void saveProfessors() {
    FILE *fp = fopen(PROFESSORS_FILE, "w");
    if (!fp) {
        perror("File opening failed");
        printf("Error opening %s for writing.\n", PROFESSORS_FILE);
        return;
    }
    for (int i = 0; i < professorCount; i++) {
        fprintf(fp, "ID: %s, Name: %s, Major: %s, Email: %s, Phone: %s, Password: %s, Courses: ",
                professors[i].id,
                professors[i].name,
                professors[i].major,
                professors[i].email,
                professors[i].phone,
                professors[i].password);
        for (int j = 0; j < professors[i].courseCount; j++) {
            fprintf(fp, "%s", professors[i].courses[j]);
            if (j < professors[i].courseCount - 1) {
                fprintf(fp, ", ");
            }
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("Professors data saved successfully to %s.\n", PROFESSORS_FILE);
}


// Login function
int login(char *role, char *id, char *password) {
    if (strcmp(role, "student") == 0) {
        for (int i = 0; i < studentCount; i++) {
            if (strcmp(students[i].id, id) == 0 && strcmp(students[i].password, password) == 0) {
                return i;
            }
        }
    } else if (strcmp(role, "professor") == 0) {
        for (int i = 0; i < professorCount; i++) {
            if (strcmp(professors[i].id, id) == 0 && strcmp(professors[i].password, password) == 0) {
                return i;
            }
        }
    }
    return -1;
}

// Function to convert numeric grade to letter grade
const char* getLetterGrade(double grade) {
    if (grade >= 4.5) return "A+";
    else if (grade >= 4.0) return "A0";
    else if (grade >= 3.5) return "B+";
    else if (grade >= 3.0) return "B0";
    else if (grade >= 2.5) return "C+";
    else if (grade >= 2.0) return "C0";
    else if (grade >= 1.5) return "D+";
    else if (grade >= 1.0) return "D0";
    else return "F";
}

// Function to display all courses and corresponding letter grades
void displayCoursesAndGrades(Student *stud) {
    printf("\n--- Courses and Grades ---\n");
    printf("Course\t\tGrade\n");
    printf("---------------------------\n");
    for (int i = 0; i < 6; i++) {
        if (strlen(stud->courses[i]) == 0)      //if no more courses
        break;
        printf("%-25s %s\n", stud->courses[i], getLetterGrade(stud->grades[i]));
    }
}

// Function to calculate GPA for a student
double calculateGPA(Student *stud) {
    double total = 0.0;
    int count = 0;
    for (int i = 0; i < 6; i++) {
        if (stud->grades[i] > 0.0) { // Assuming a grade of 0.0 means no grade assigned
            total += stud->grades[i];
            count++;
        }
    }
    if (count == 0) return 0.0; // Avoid division by zero
    return total / count;
}

void adminMenu() {
    int choice;
    while (1) {
        printf("\n--- Admin Menu ---\n1. View Information\n2. Change Information\n3. Encrypt Data\n4. Decrypt Data\n5. Add New Student\n6. Delete Student\n7. Logout\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline
        if (choice == 7) {
            printf("Logging out...\n");
            break;
        }
        switch (choice) {
            case 1:
                viewInformationAdmin();
                break;
            case 2:
                changeInformationAdmin();
                break;
            case 3:
                encryptData();
                break;
            case 4:
                decryptData();
                break;
            case 5:
                addNewStudent();
                break;
            case 6:
                deleteStudent();
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

void professorMenu(Professor *prof) {
    int choice;
    while (1) {
        printf("\n--- Professor Menu ---\n1. Change Email/Phone Number\n2. View All Students' Information\n3. Generate Student Report\n4. Filter Students\n5. Add New Student\n6. Logout\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline
        if (choice == 6) {
            printf("Logging out...\n");
            break;
        }
        switch (choice) {
            case 1:
                changeProfessorInfo(prof);
                break;
            case 2:
                viewStudentsProfessor(prof);
                break;
            case 3:
                generateStudentReport();
                break;
            case 4:
                filterStudentsProfessor();
                break;
            case 5:
                addNewStudent();
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

void studentMenu(Student *stud) {
    int choice;
    while (1) {
        printf("\n--- Student Menu ---\n1. View My Information\n2. Change Email/Phone Number\n3. Logout\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline
        if (choice == 3) {
            printf("Logging out...\n");
            break;
        }
        switch (choice) {
            case 1:
                viewStudentInfo(stud);
                break;
            case 2:
                changeStudentInfo(stud);
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

void addNewStudent() {
    if (studentCount >= MAX_STUDENTS) {
        printf("Maximum number of students reached. Cannot add more.\n");
        return;
    }
    Student newStudent;
    char temp[MAX_STRING]; // Temporary buffer for input

    printf("Enter Name: ");         //all inputs are listed below
    fgets(newStudent.name, MAX_STRING, stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = '\0';
    if (!validateName(newStudent.name)) {
        printf("Invalid name. Only alphabetic characters are allowed.\n");
        return;
    }
    printf("Enter Email: ");
    fgets(newStudent.email, MAX_STRING, stdin);
    newStudent.email[strcspn(newStudent.email, "\n")] = '\0';
    if (!validateEmail(newStudent.email)) {
        printf("Invalid email. Must contain '@inha.edu'.\n");
        return;
    }
    printf("Enter Phone Number: ");
    fgets(newStudent.phoneNumber, MAX_STRING, stdin);
    newStudent.phoneNumber[strcspn(newStudent.phoneNumber, "\n")] = '\0';
    if (!validatePhoneNumber(newStudent.phoneNumber)) {
        printf("Invalid phone number. Must be 11 digits starting with '010'.\n");
        return;
    }
    printf("Enter Major (IBT, ISE, KLC): ");
    fgets(newStudent.faculty, MAX_STRING, stdin);
    newStudent.faculty[strcspn(newStudent.faculty, "\n")] = '\0';
    if (!validateMajor(newStudent.faculty)) {
        printf("Invalid major. Choose from IBT, ISE, KLC.\n");
        return;
    }
    printf("Enter Password: ");
    fgets(newStudent.password, MAX_STRING, stdin);
    newStudent.password[strcspn(newStudent.password, "\n")] = '\0';
    if (!validatePassword(newStudent.password)) {
        printf("Invalid password. Must be exactly 10 characters.\n");
        return;
    }
    printf("Enter Date of Birth (yyyymmdd): ");
    fgets(newStudent.dateOfBirth, MAX_STRING, stdin);
    newStudent.dateOfBirth[strcspn(newStudent.dateOfBirth, "\n")] = '\0';
    if (!validateBirthday(newStudent.dateOfBirth)) {
        printf("Invalid date of birth. Must be in 'yyyymmdd' format.\n");
        return;
    }
    printf("Enter Address: ");
    fgets(newStudent.address, MAX_STRING, stdin);
    newStudent.address[strcspn(newStudent.address, "\n")] = '\0';
    while (1) {
        printf("Enter Gender (M/F/U): ");
        fgets(temp, MAX_STRING, stdin);
        temp[strcspn(temp, "\n")] = '\0';
        if (strlen(temp) == 1 && (toupper(temp[0]) == 'M' || toupper(temp[0]) == 'F' || toupper(temp[0]) == 'U')) {
            newStudent.gender = toupper(temp[0]);
            break;
        } else {
            printf("Invalid input. Please enter 'M' for Male, 'F' for Female, or 'U' for Unspecified.\n");
        }
    }
    while (1) {
        printf("Enter Scholarship Amount (percentage, e.g., 100 or 50): ");
        fgets(temp, MAX_STRING, stdin);
        newStudent.scholarshipAmount = atoi(temp);
        if (newStudent.scholarshipAmount >= 0 && newStudent.scholarshipAmount <= 100) {
            break;
        } else {
            printf("Invalid input. Please enter a percentage between 0 and 100.\n");
        }
    }
    printf("Enter Nationality: ");
    fgets(newStudent.nationality, MAX_STRING, stdin);
    newStudent.nationality[strcspn(newStudent.nationality, "\n")] = '\0';
    if (strlen(newStudent.nationality) == 0) {
        strcpy(newStudent.nationality, "Unknown");
    }
    while (1) {
        printf("Is the student attending any clubs? (Y/N): ");
        fgets(temp, MAX_STRING, stdin);
        temp[strcspn(temp, "\n")] = '\0';
        if (strcasecmp(temp, "Y") == 0) {
            printf("Enter Club Name: ");
            fgets(newStudent.clubsAttending, MAX_STRING, stdin);
            newStudent.clubsAttending[strcspn(newStudent.clubsAttending, "\n")] = '\0';
            if (strlen(newStudent.clubsAttending) == 0) {
                strcpy(newStudent.clubsAttending, "None");
            }
            break;
        } else if (strcasecmp(temp, "N") == 0) {
            strcpy(newStudent.clubsAttending, "None");
            break;
        } else {
            printf("Invalid input. Please enter 'Y' for Yes or 'N' for No.\n");
        }
    }

    int lastID = 0;                                   //there is no option to choose unique id, it will be automatically generated by adding 1 to the last in the list
    for (int i = 0; i < studentCount; i++) {
        int idNum = atoi(students[i].id);
        if (idNum > lastID) {
            lastID = idNum;
        }
    }
    lastID++;
    sprintf(newStudent.id, "%d", lastID);
    newStudent.semesters = 1;

    int profIndices[MAX_PROFESSORS];                 //advisor professor is assigned randomly
    int profCount = 0;
    for (int i = 0; i < professorCount; i++) {
        if (strcmp(professors[i].major, newStudent.faculty) == 0) {
            profIndices[profCount++] = i;
        }
    }
    if (profCount == 0) {
        printf("No professors available for this major. Cannot assign advisor.\n");
        return;
    }
    srand(time(NULL));
    int randIdx = rand() % profCount;
    strcpy(newStudent.advisorName, professors[profIndices[randIdx]].name);
    strcpy(newStudent.advisorID, professors[profIndices[randIdx]].id);

    for(int i = 0; i < 6; i++) {                    //no grades and no courses
        strcpy(newStudent.courses[i], "");
        newStudent.grades[i] = 0.0;
    }
    students[studentCount] = newStudent;            //adding new student to the main students array
    studentCount++;
    saveStudents();
    printf("New student added successfully with ID: %s\n", newStudent.id);
}

void deleteStudent() {
    char targetID[MAX_STRING];
    printf("\n--- Delete Student ---\n");
    printf("Enter the Student ID to delete: ");
    fgets(targetID, MAX_STRING, stdin);
    targetID[strcspn(targetID, "\n")] = '\0'; // Remove newline character

    // Search for the student by ID
    int index = -1;
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].id, targetID) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Student with ID '%s' not found.\n", targetID);
        return;
    }

    char confirmation[MAX_STRING];              //confirmation of deletion asking User
    printf("Are you sure you want to delete student '%s' (ID: %s)? (Y/N): ", students[index].name, students[index].id);
    fgets(confirmation, MAX_STRING, stdin);
    confirmation[strcspn(confirmation, "\n")] = '\0'; // Remove newline character

    if (strcasecmp(confirmation, "Y") != 0 && strcasecmp(confirmation, "YES") != 0) {
        printf("Deletion cancelled.\n");
        return;
    }
    for (int i = index; i < studentCount - 1; i++) {      // Shift students in the array to overwrite the deleted student
        students[i] = students[i + 1];
    }

    // Clear the last student entry (optional)                                          what is it?
    memset(&students[studentCount - 1], 0, sizeof(Student));

    // Decrement student count
    studentCount--;

    saveStudents();
    printf("Student '%s' (ID: %s) has been successfully deleted.\n", targetID, students[index].name);
}

void viewInformationAdmin() {
    int choice;
    printf("1. View Student Information\n");
    printf("2. View Professor Information\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar();
    if (choice == 1) {
        char studentID[MAX_STRING];
        printf("Enter student ID: ");
        fgets(studentID, MAX_STRING, stdin);
        studentID[strcspn(studentID, "\n")] = '\0';
        for (int i = 0; i < studentCount; i++) {
            if (strcmp(students[i].id, studentID) == 0) {
                printf("ID: %s\nName: %s\nFaculty: %s\nAdvisor: %s\nEmail: %s\nPhone: %s\nDOB: %s\nGender: %c\nAddress: %s\nSemesters: %d\n",
                    students[i].id,
                    students[i].name,
                    students[i].faculty,
                    students[i].advisorName,
                    students[i].email,
                    students[i].phoneNumber,
                    students[i].dateOfBirth,
                    students[i].gender,
                    students[i].address,
                    students[i].semesters);
                displayCoursesAndGrades(&students[i]);
                double gpa = calculateGPA(&students[i]);
                printf("GPA: %.2f\n", gpa);
                return;
            }
        }
        printf("Student not found.\n");
    } else if (choice == 2) {
        char profID[MAX_STRING];
        printf("Enter professor ID: ");
        fgets(profID, MAX_STRING, stdin);
        profID[strcspn(profID, "\n")] = '\0';
        for (int i = 0; i < professorCount; i++) {
            if (strcmp(professors[i].id, profID) == 0) {
                printf("ID: %s\nName: %s\nMajor: %s\nEmail: %s\nPhone: %s\n",
                    professors[i].id,
                    professors[i].name,
                    professors[i].major,
                    professors[i].email,
                    professors[i].phone);
                return;
            }
        }
        printf("Professor not found.\n");
    } else {
        printf("Invalid choice.\n");
    }
}

void changeInformationAdmin() {
    int choice;
    printf("1. Change Student Information\n");
    printf("2. Change Professor Information\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar();
    if (choice == 1) {
        char studentID[MAX_STRING];
        printf("Enter student ID: ");
        fgets(studentID, MAX_STRING, stdin);
        studentID[strcspn(studentID, "\n")] = '\0';
        for (int i = 0; i < studentCount; i++) {
            if (strcmp(students[i].id, studentID) == 0) {
                printf("1. Change Email\n2. Change Phone Number\n3. Change Address\nEnter your choice: ");
                int opt;
                scanf("%d", &opt);
                getchar();
                if (opt == 1) {
                    printf("Enter new email: ");
                    fgets(students[i].email, MAX_STRING, stdin);
                    students[i].email[strcspn(students[i].email, "\n")] = '\0';
                } else if (opt == 2) {
                    printf("Enter new phone number: ");
                    fgets(students[i].phoneNumber, MAX_STRING, stdin);
                    students[i].phoneNumber[strcspn(students[i].phoneNumber, "\n")] = '\0';
                } else if (opt == 3) {
                    printf("Enter new address: ");
                    fgets(students[i].address, MAX_STRING, stdin);
                    students[i].address[strcspn(students[i].address, "\n")] = '\0';
                } else {
                    printf("Invalid option.\n");
                    return;
                }
                saveStudents();
                printf("Information updated.\n");
                return;
            }
        }
        printf("Student not found.\n");
    } else if (choice == 2) {
        char profID[MAX_STRING];
        printf("Enter professor ID: ");
        fgets(profID, MAX_STRING, stdin);
        profID[strcspn(profID, "\n")] = '\0';
        for (int i = 0; i < professorCount; i++) {
            if (strcmp(professors[i].id, profID) == 0) {
                printf("1. Change Email\n2. Change Phone Number\nEnter your choice: ");
                int opt;
                scanf("%d", &opt);
                getchar();
                if (opt == 1) {
                    printf("Enter new email: ");
                    fgets(professors[i].email, MAX_STRING, stdin);
                    professors[i].email[strcspn(professors[i].email, "\n")] = '\0';
                } else if (opt == 2) {
                    printf("Enter new phone number: ");
                    fgets(professors[i].phone, MAX_STRING, stdin);
                    professors[i].phone[strcspn(professors[i].phone, "\n")] = '\0';
                } else {
                    printf("Invalid option.\n");
                    return;
                }
                // Save professors function needed
                saveProfessors();
                printf("Information updated.\n");
                return;
            }
        }
        printf("Professor not found.\n");
    } else {
        printf("Invalid choice.\n");
    }
}

void encryptData() {
    int choice;
    printf("1. Encrypt Passwords\n2. Encrypt Addresses\n3. Encrypt Phone Numbers\n4. Encrypt Emails\nEnter your choice: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }
    getchar(); // Consume newline

    int key;
    for (int i = 0; i < studentCount; i++) {
        int idLength = strlen(students[i].id);
        // Calculate key as the sum of the last two digits
        int digit1 = students[i].id[idLength - 1] - '0';
        int digit2 = students[i].id[idLength - 2] - '0';
        key = digit1 + digit2;

        if (choice == 1) {
            encrypt(students[i].password, key);
        } else if (choice == 2) {
            encrypt(students[i].address, key);
        } else if (choice == 3) {
            encrypt(students[i].phoneNumber, key);
        } else if (choice == 4) {
            encrypt(students[i].email, key);
        } else {
            printf("Invalid choice.\n");
            return;
        }
    }
    saveStudents();
    printf("Data encrypted.\n");
}

void decryptData() {
    int choice;
    printf("1. Decrypt Passwords\n2. Decrypt Addresses\n3. Decrypt Phone Numbers\n4. Decrypt Emails\nEnter your choice: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }
    getchar(); // Consume newline

    int key;
    for (int i = 0; i < studentCount; i++) {
        int idLength = strlen(students[i].id);
        // Calculate key as the sum of the last two digits
        int digit1 = students[i].id[idLength - 1] - '0';
        int digit2 = students[i].id[idLength - 2] - '0';
        key = digit1 + digit2;

        if (choice == 1) {
            decrypt(students[i].password, key);
        } else if (choice == 2) {
            decrypt(students[i].address, key);
        } else if (choice == 3) {
            decrypt(students[i].phoneNumber, key);
        } else if (choice == 4) {
            decrypt(students[i].email, key);
        } else {
            printf("Invalid choice.\n");
            return;
        }
    }
    saveStudents();
    printf("Data decrypted.\n");
}




// Change professor info
void changeProfessorInfo(Professor *prof) {
    int choice;
    printf("1. Change Email\n2. Change Phone Number\nEnter your choice: ");
    scanf("%d", &choice);
    getchar();
    if (choice == 1) {
        printf("Enter new email: ");
        fgets(prof->email, MAX_STRING, stdin);
        prof->email[strcspn(prof->email, "\n")] = '\0';
    } else if (choice == 2) {
        printf("Enter new phone number: ");
        fgets(prof->phone, MAX_STRING, stdin);
        prof->phone[strcspn(prof->phone, "\n")] = '\0';
    } else {
        printf("Invalid choice.\n");
        return;
    }
    saveProfessors();       //чатгпт решил не писать эту часть
    printf("Information updated.\n");
}

// View students as professor with courses and GPA
void viewStudentsProfessor(Professor *prof) {
    int found = 0;
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].advisorID, prof->id) == 0) {
            found = 1;
            printf("\nID: %s, Name: %s\n", students[i].id, students[i].name);
            
            // Display Courses and Grades
            displayCoursesAndGrades(&students[i]);

            // Calculate and display GPA
            double gpa = calculateGPA(&students[i]);
            printf("GPA: %.2f\n", gpa);
        }
    }
    if (!found) {
        printf("No students found under your advisement.\n");
    }
}


// Generate student report
void generateStudentReport() {
    char studentID[MAX_STRING];
    printf("Enter student ID: ");
    fgets(studentID, MAX_STRING, stdin);
    studentID[strcspn(studentID, "\n")] = '\0';
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].id, studentID) == 0) {
            char filename[MAX_STRING];
            sprintf(filename, "%s_report.txt", studentID);
            FILE *fp = fopen(filename, "w");
            if (!fp) {
                perror("File opening failed");
                printf("Error creating report file.\n");
                return;
            }
            fprintf(fp, "ID: %s\nName: %s\nFaculty: %s\nAdvisor: %s\nEmail: %s\nPhone: %s\nDOB: %s\nGender: %c\nAddress: %s\nSemesters: %d\n",
                    students[i].id,
                    students[i].name,
                    students[i].faculty,
                    students[i].advisorName,
                    students[i].email,
                    students[i].phoneNumber,
                    students[i].dateOfBirth,
                    students[i].gender,
                    students[i].address,
                    students[i].semesters);
            fclose(fp);
            printf("Report generated: %s\n", filename);
            return;
        }
    }
    printf("Student not found.\n");
}

// Filter students as professor
void filterStudentsProfessor() {
    int choice;
    printf("Filter by:\n");
    printf("1. Department\n");
    printf("2. Scholarship Amount\n");
    printf("3. Country\n");
    printf("4. Semester\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar(); // Consume newline

    if (choice == 1) {
        char dept[MAX_STRING];
        printf("Enter department: ");
        fgets(dept, MAX_STRING, stdin);
        dept[strcspn(dept, "\n")] = '\0';
        printf("\n--- Students in Department: %s ---\n", dept);
        int found = 0;
        for (int i = 0; i < studentCount; i++) {
            if (strcmp(students[i].faculty, dept) == 0) {
                printf("ID: %s, Name: %s\n", students[i].id, students[i].name);
                found++;
            }
        }
        if (found == 0) {
            printf("No students found in the specified department.\n");
        }
    } 
    else if (choice == 2) {
        int scholarship;
        printf("Enter scholarship amount (percentage, e.g., 100 or 50): ");
        scanf("%d", &scholarship);
        getchar(); // Consume newline

        if (scholarship < 0 || scholarship > 100) {
            printf("Invalid scholarship amount. Please enter a value between 0 and 100.\n");
            return;
        }

        printf("\n--- Students with %d%% Scholarship ---\n", scholarship);
        int found = 0;
        for (int i = 0; i < studentCount; i++) {
            if (students[i].scholarshipAmount == scholarship) {
                printf("ID: %s, Name: %s\n", students[i].id, students[i].name);
                found++;
            }
        }
        if (found == 0) {
            printf("No students found with the specified scholarship amount.\n");
        }
    } 
    else if (choice == 3) {
        char country[MAX_STRING];
        printf("Enter country: ");
        fgets(country, MAX_STRING, stdin);
        country[strcspn(country, "\n")] = '\0';
        printf("\n--- Students from Country: %s ---\n", country);
        int found = 0;
        for (int i = 0; i < studentCount; i++) {
            if (strcmp(students[i].nationality, country) == 0) {
                printf("ID: %s, Name: %s\n", students[i].id, students[i].name);
                found++;
            }
        }
        if (found == 0) {
            printf("No students found from the specified country.\n");
        }
    } 
    else if (choice == 4) {
        int sem;
        printf("Enter semester: ");
        scanf("%d", &sem);
        getchar(); // Consume newline

        if (sem < 1) {
            printf("Invalid semester number. Please enter a positive integer.\n");
            return;
        }

        printf("\n--- Students in Semester: %d ---\n", sem);
        int found = 0;
        for (int i = 0; i < studentCount; i++) {
            if (students[i].semesters == sem) {
                printf("ID: %s, Name: %s\n", students[i].id, students[i].name);
                found++;
            }
        }
        if (found == 0) {
            printf("No students found in the specified semester.\n");
        }
    } 
    else {
        printf("Invalid choice. Please select a valid option.\n");
    }
}


// View student info including courses and GPA
void viewStudentInfo(Student *stud) {
    printf("ID: %s\n", stud->id);
    printf("Name: %s\n", stud->name);
    printf("Faculty: %s\n", stud->faculty);
    printf("Advisor: %s\n", stud->advisorName);
    printf("Email: %s\n", stud->email);
    printf("Phone: %s\n", stud->phoneNumber);
    printf("DOB: %s\n", stud->dateOfBirth);
    printf("Gender: %c\n", stud->gender);
    printf("Address: %s\n", stud->address);
    printf("Semesters: %d\n", stud->semesters);
    printf("Scholarship Amount: %d%%\n", stud->scholarshipAmount);
    printf("Clubs Attending: %s\n", stud->clubsAttending);
    printf("Nationality: %s\n", stud->nationality);
    
    // Display Courses and Grades
    displayCoursesAndGrades(stud);

    // Calculate and display GPA
    double gpa = calculateGPA(stud);
    printf("GPA: %.2f\n", gpa);
}


// Change student info
void changeStudentInfo(Student *stud) {
    int choice;
    printf("1. Change Email\n2. Change Phone Number\nEnter your choice: ");
    scanf("%d", &choice);
    getchar();
    if (choice == 1) {
        printf("Enter new email: ");
        fgets(stud->email, MAX_STRING, stdin);
        stud->email[strcspn(stud->email, "\n")] = '\0';
    } else if (choice == 2) {
        printf("Enter new phone number: ");
        fgets(stud->phoneNumber, MAX_STRING, stdin);
        stud->phoneNumber[strcspn(stud->phoneNumber, "\n")] = '\0';
    } else {
        printf("Invalid choice.\n");
        return;
    }
    saveStudents();
    printf("Information updated.\n");
}

// Utility functions
int validateEmail(char *email) {
    return strstr(email, "@inha.edu") != NULL;
}

int validatePhoneNumber(char *phone) {
    return strlen(phone) == 11 && strncmp(phone, "010", 3) == 0;
}

int validatePassword(char *password) {
    return strlen(password) == 10;
}

int validateName(char *name) {
    for (int i = 0; name[i]; i++) {
        if (!isalpha(name[i])) return 0;
    }
    return 1;
}

int validateBirthday(char *birthday) {
    return strlen(birthday) == 8;
}

int validateMajor(char *major) {
    return strcmp(major, "IBT") == 0 || strcmp(major, "ISE") == 0 || strcmp(major, "KLC") == 0;
}

void encrypt(char *text, int key) {
    for (int i = 0; text[i]; i++) {
        text[i] += key;
    }
}

void decrypt(char *text, int key) {
    for (int i = 0; text[i]; i++) {
        text[i] -= key;
    }
}
