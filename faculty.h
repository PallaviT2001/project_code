#ifndef FACULTY_H
#define FACULTY_H

struct Faculty {
    int id;
    char name[50];
    char department[50];
    int age;
    char qualification[50];
    struct Faculty *next;
};
extern struct Faculty *facultyHead;

void insertFaculty(int id, const char *name, const char *department, int age, const char *qualification);
void deleteFaculty(int id);
void updateFaculty(int id);
void displayFacultyDetails();
void sortFacultiesByID();
void sortFacultiesByName();
void searchFacultyById(int id);
void addFacultyToList(struct Faculty *newFaculty);
int getTotalFacultyCount();
struct Faculty* mergeFacultySortedLists(struct Faculty* left, struct Faculty* right);
struct Faculty* getFacultyMiddle(struct Faculty* head);

#endif
