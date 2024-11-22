#include "student.h"
#include <assert.h>

void test_student_add(void) {
    Student student;
    student_init_structure(&student);
    strcpy(student.id, "12345");
    strcpy(student.name, "John Doe");
    strcpy(student.department, "Computer Science");

    StatusCode status = student_add(&student);
    assert(status == SUCCESS);

    Student result;
    status = student_get("12345", &result);
    assert(status == SUCCESS);
    assert(strcmp(result.name, "John Doe") == 0);
}

int main(void) {
    test_student_add();
    printf("All tests passed!\n");
    return 0;
}
