#ifndef STUDENT_H
#define STUDENT_H

struct Student {
    int id;
    char name[50];
    int age;
    char contactNumber[20];
    struct Student *next;
};
extern struct Student *studentHead;

void insertStudent(int id, const char *name, int age, const char *contactNumber);
void deleteStudent(int id);
void updateStudent(int id);
void displayStudentDetails();
void sortStudentsByID();
void sortStudentsByName();
void addStudentToList(struct Student *newStudent);
int getTotalStudentCount();
void searchStudentById(int id);

struct Student* mergeSortedLists(struct Student* left, struct Student* right);
struct Student* getMiddle(struct Student* head);

#endif
